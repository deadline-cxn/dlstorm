/* Seth's File Cabinet Class */

#include "c_gaf.h"

//extern "C" void    Log(char *fmt,...);

//////////////////////////////////////////////////////////////////////////////

CCab::CDirScanner::CDirScanner()
{
	Handle=INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////

CCab::CDirScanner::~CDirScanner()
{
	if(Handle!=INVALID_HANDLE_VALUE)FindClose(Handle);
}

//////////////////////////////////////////////////////////////////////////////

void CCab::CDirScanner::Start(LPSTR dirname)
{
	strcpy(DirName,dirname);
	strcat(DirName,"*.*");
	NextIsFirst=true;
	GetFile();
	GetFile();
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::CDirScanner::GetFile()
{
	if(NextIsFirst)
	{
		Handle=FindFirstFile(DirName,&FindData);
		NextIsFirst=false;
		if(Handle!=INVALID_HANDLE_VALUE)return true;
	}else
	{
		if(FindNextFile(Handle,&FindData))return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::CDirScanner::Error()
{
	if(Handle==INVALID_HANDLE_VALUE)return true;
	return false;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BUFFERSIZE (1024*1024)

DWORD NukeFile_Version=0x020101c5;

//////////////////////////////////////////////////////////////////////////////

CCab::CCab()
{
	CompLevel=NFCOMP_NONE;
	CurrentFileName[0]=0;
	FileOpen=false;
	NumElements=0;
	MaxElements=1;
	Elements=(NukeFile_ElmHeader*)malloc(sizeof(NukeFile_ElmHeader)*MaxElements);
	SetFileDescription("--FileCab Class--");
}

//////////////////////////////////////////////////////////////////////////////

CCab::~CCab()
{
	free(Elements);
	Close();
}

//////////////////////////////////////////////////////////////////////////////

// _bIgnoreDescription = true to ignore the NUK file's Description.
bool CCab::Open ( LPSTR fn, bool _bIgnoreDescription )
{
	Close();
	strcpy(CurrentFileName,fn);
	FileOpen=false;
	fh=fopen(fn,"r+b");
    if(fh==NULL)
    {
        Create(fn);
        fh=fopen(fn,"r+b");
        if(fh==NULL)
            return false;
    }
    FileOpen=ReadFile ( _bIgnoreDescription );
	if(!FileOpen)fclose(fh);
	return FileOpen;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::Create(LPSTR fn)
{
	Close();
	FileOpen=false;
	fh=fopen(fn,"w+b");
	if(fh==NULL)return false;
	Header.Size=sizeof(Header);
	Header.Version=NukeFile_Version;
	Header.NumElements=0;
	strcpy(Header.Description,FileDesc);
	WriteHeader();
	fclose(fh);
	return Open(fn);
}

//////////////////////////////////////////////////////////////////////////////

// _bIgnoreDescription = true to ignore the NUK file's Description.
bool CCab::ReadFile(bool _bIgnoreDescription)
{
	NukeFile_ElmHeader TempElement;
	fseek(fh,0,SEEK_SET);
	fread(&Header,sizeof(Header),1,fh);
	if(Header.Version!=NukeFile_Version)return false;
	if(Header.Size!=sizeof(Header))return false;


    // Ignore the header if the user want's us to.
    if ( _bIgnoreDescription == false )
    {
        if(strcmpi(Header.Description,FileDesc)!=0)return false;
    }                                   // EO: if


	NumElements=0;
	SetAmount(Header.NumElements);
	for(int n=0;n<Header.NumElements;n++)
	{
		int fpos=ftell(fh);
		fread(&TempElement,sizeof(TempElement),1,fh);
		TempElement.FileOffset=ftell(fh);
		AddElement(&TempElement);
		fseek(fh,fpos,SEEK_SET);
		fwrite(&TempElement,sizeof(TempElement),1,fh);
		fseek(fh,TempElement.FileSize,SEEK_CUR);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::AddElement(NukeFile_ElmHeader * Element)
{
	if(NumElements==MaxElements)
	{
		SetAmount(MaxElements+1);
	}
	Elements[NumElements]=*Element;
	NumElements++;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::SetAmount(int a)
{
	MaxElements=a;
	Elements=(NukeFile_ElmHeader*)realloc(Elements,sizeof(NukeFile_ElmHeader)*MaxElements);
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::Close()
{
	if(FileOpen)fclose(fh);
	FileOpen=false;
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::CreateFile(LPSTR Name, DWORD Size)
{
	if(!FileOpen)return false;
	if(FindFile(Name)>=0)return false;
	char FileName[NF_NAMESIZE];
	char Dir[NF_NAMESIZE];
	int DirNumber=0;
	if(SplitNameAndDir(Name,FileName,Dir))
	{
		DirNumber=FindDirNumber(Dir);
		if(DirNumber<0)return false;
	}
	int StartPos=GetFullLength();
	fseek(fh,StartPos,SEEK_SET);
	NukeFile_ElmHeader TempElement;
	TempElement.DirPos=DirNumber;
	TempElement.DirNumber=0;
	TempElement.FileOffset=StartPos+sizeof(TempElement);
	TempElement.FileSize=Size;
	TempElement.UncompSize=Size;
	TempElement.Type=NFELMTYPE_FILE;
	TempElement.CompressLevel=CompLevel;
	ZeroMemory(TempElement.Name,sizeof(TempElement.Name));
	strcpy(TempElement.Name,FileName);
	fwrite(&TempElement,sizeof(TempElement),1,fh);
	AddElement(&TempElement);
	Header.NumElements=NumElements;
	WriteHeader();
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::CreateCompFile(LPSTR Name, DWORD CompSize, DWORD Size, DWORD clevel)
{
	if(!FileOpen)return false;
	if(FindFile(Name)>=0)return false;
	char FileName[NF_NAMESIZE];
	char Dir[NF_NAMESIZE];
	int DirNumber=0;
	if(SplitNameAndDir(Name,FileName,Dir))
	{
		DirNumber=FindDirNumber(Dir);
		if(DirNumber<0)return false;
	}
	int StartPos=GetFullLength();
	fseek(fh,StartPos,SEEK_SET);
	NukeFile_ElmHeader TempElement;
	TempElement.DirPos=DirNumber;
	TempElement.DirNumber=0;
	TempElement.FileOffset=StartPos+sizeof(TempElement);
	TempElement.FileSize=CompSize;
	TempElement.UncompSize=Size;
	TempElement.Type=NFELMTYPE_FILE;
	TempElement.CompressLevel=clevel;
	ZeroMemory(TempElement.Name,sizeof(TempElement.Name));
	strcpy(TempElement.Name,FileName);
	fwrite(&TempElement,sizeof(TempElement),1,fh);
	AddElement(&TempElement);
	Header.NumElements=NumElements;
	WriteHeader();
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::CheckSlash(char c)
{
	if(c=='/'||c=='\\')return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::GetNumSlashes(LPSTR String)
{
	int sl=strlen(String);
	int NumSlashes=0;
	for(int n=0;n<sl;n++)
	{
		if(CheckSlash(String[n]))NumSlashes++;
	}
	return NumSlashes;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::GetUntilSlash(LPSTR Source,LPSTR Dest)
{
	int p=0;
	do
	{
		Dest[p]=Source[p];
		p++;
	}while(!CheckSlash(Source[p])&&Source[p]!=0);
	Dest[p]=0;
	return p;
}

//////////////////////////////////////////////////////////////////////////////

FILE * CCab::Seek(LPSTR Name)
{
	if(!FileOpen)return NULL;
	int fn=FindFile(Name);
	if(fn<0)return NULL;
	fseek(fh,Elements[fn].FileOffset,SEEK_SET);
	return fh;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::CreateDir(LPSTR Name)
{
	if(!FileOpen)return false;
	if(FindDir(Name)>=0)return false;
	char FileName[NF_NAMESIZE];
	char Dir[NF_NAMESIZE];
	int DestDir=0;
	if(SplitNameAndDir(Name,FileName,Dir))
	{
		DestDir=FindDirNumber(Dir);
		if(DestDir<0)return false;
	}
	int StartPos=GetFullLength();
	fseek(fh,StartPos,SEEK_SET);
	NukeFile_ElmHeader TempElement;
	TempElement.DirPos=DestDir;
	TempElement.DirNumber=FindAvailDir();
	TempElement.FileOffset=StartPos+sizeof(TempElement);
	TempElement.FileSize=0;
	TempElement.UncompSize=0;
	TempElement.Type=NFELMTYPE_DIR;
	TempElement.CompressLevel=NFCOMP_NONE;
	ZeroMemory(TempElement.Name,sizeof(TempElement.Name));
	strcpy(TempElement.Name,FileName);
	fwrite(&TempElement,sizeof(TempElement),1,fh);
	AddElement(&TempElement);
	Header.NumElements=NumElements;
	WriteHeader();
	return true;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::FindAvailDir()
{
	int fd=1;
	bool done;
	do
	{
		done=true;
		for(int n=0;n<NumElements;n++)
		{
			if(Elements[n].DirNumber==fd)done=false;
		}
		if(!done)fd++;
	}while(!done);
	return fd;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::AddFile(LPSTR Name,LPSTR filename)
{

#ifdef _FILECAB_SHOW_PROG

	printf("   %s\n",Name);

#endif


	if(CompLevel!=NFCOMP_NONE)return AddFile_Compress(Name,filename);
	FILE *f=fopen(filename,"rb");
	if(f==NULL)
	{
		return false;
	}
	int size=FileSize(f);
	if(!CreateFile(Name,size))
	{
		fclose(f);
		return false;
	}
	if(Seek(Name)==NULL)
	{
		fclose(f);
		return false;
	}
	CopyData(f,fh,size);
	fclose(f);
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::AddFileFromMem(LPSTR Name,unsigned char *fb,int size)
{
    if(!CreateFile(Name,size)) return false;
    if(!Seek(Name)) return false;
    fwrite(fb,1,size,fh);
    return true;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::FileSize(FILE * f)
{
	int Pos;
	int Size;
	Pos=ftell(f);
	fseek(f,0,SEEK_END);
	Size=ftell(f);
	fseek(f,Pos,SEEK_SET);
	return Size;
}

//////////////////////////////////////////////////////////////////////////////

//This Returns false every time....
bool CCab::WriteHeader()
{
	fseek(fh,0,SEEK_SET);
    if ( fwrite ( &Header, 1, sizeof(Header), fh) == sizeof(Header) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::RemoveFile(LPSTR Name)
{
    if ( !FileOpen )
    {
        return false;
    }

	int ne=FindFile(Name);

    if ( ne==-1 )
    {
        return false;
    }

	fseek(fh,0,SEEK_SET);
	int FullLength=GetFullLength();

	FILE *tf=tmpfile();

	CopyData(fh,tf,Elements[ne].FileOffset-sizeof(NukeFile_ElmHeader));

	fseek(fh,Elements[ne].FileOffset+Elements[ne].FileSize,SEEK_SET);

	CopyData(fh,tf,FullLength-ftell(fh));

	fclose(fh);

	fh=fopen(CurrentFileName,"w+b");

    if ( fh == NULL )
    {
        return false;
    }

	Header.NumElements--;

	fseek(tf,0,SEEK_SET);

	CopyData(tf,fh,FileSize(tf));

	WriteHeader();

	ReadFile();

	fclose(tf);

	return true;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::CopyData(FILE * Source, FILE * Dest, int Size)
{
	void *buffer=malloc(BUFFERSIZE);
	if(Size>BUFFERSIZE)
	{
		do
		{
			fread(buffer,1,BUFFERSIZE,Source);
			fwrite(buffer,1,BUFFERSIZE,Dest);
			Size-=BUFFERSIZE;
		}while(Size>=BUFFERSIZE);
	}
	if(Size>0)
	{
		fread(buffer,1,Size,Source);
		fwrite(buffer,1,Size,Dest);
	}
	free(buffer);
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::SplitNameAndDir(LPSTR Source, LPSTR FileName, LPSTR Dir)
{
	int sl=strlen(Source);
	int p=sl;
	do
	{
		p--;
	}while(!CheckSlash(Source[p])&&Source[p]!=0&&p>=0);
	int n,dp=0;
	for(n=p+1;n<=sl;n++)
	{
		FileName[dp]=Source[n];
		dp++;
	}
	FileName[dp]=0;
	dp=0;
	for(n=0;n<=p;n++)
	{
		Dir[dp]=Source[n];
		dp++;
	}
	Dir[dp]=0;
	int dl=strlen(Dir);
	if(CheckSlash(Dir[dl-1]))Dir[dl-1]=0;
	if(strlen(Dir)>0)return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::ScanTree(LPSTR Name,NF_SCANCALLBACK CallBack)
{
	if(!FileOpen)return false;
	if(CallBack==NULL)return false;
	int nd=FindDirNumber(Name);
	if(nd<0)return false;
	return ScanTreeEx(nd,CallBack);
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::ScanDir(LPSTR Name,NF_SCANCALLBACK CallBack)
{
	if(!FileOpen)return false;
	if(CallBack==NULL)return false;

	int nd=FindDirNumber(Name);

	if(nd<0)
		return false;

	char Dir[NF_NAMESIZE];
	char FileName[NF_NAMESIZE];
	char FullPath[NF_NAMESIZE];

	for(int n=0;n<NumElements;n++)
	{
		if(Elements[n].DirPos==nd)
		{
			GetFullPath(n,FullPath);
			SplitNameAndDir(FullPath,FileName,Dir);
			CallBack(&Elements[n],FullPath);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::GetFullPath(int n, LPSTR Dest)
{
	if(n>=NumElements)return false;
	Dest[0]=0;
	char FullName[NF_NAMESIZE];
	char TempString[NF_NAMESIZE];
	strcpy(FullName,Elements[n].Name);
	int DirNumber=Elements[n].DirPos;
	int ElmNumber=0;
	int e;
	for(e=0;e<NumElements;e++)
	{
		if(Elements[e].DirNumber==DirNumber)ElmNumber=e;
	}
	while(DirNumber!=0)
	{
		strcpy(TempString,Elements[ElmNumber].Name);
		strcat(TempString,"/");
		strcat(TempString,FullName);
		strcpy(FullName,TempString);
		DirNumber=Elements[ElmNumber].DirPos;
		for(e=0;e<NumElements;e++)
		{
			if(Elements[e].DirNumber==DirNumber)ElmNumber=e;
		}
	}
	strcpy(TempString,FullName);
	strcpy(FullName,TempString);
	strcpy(Dest,FullName);
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::RemoveDir(LPSTR Name)
{
	int ne=FindDir(Name);
	if(ne<0)return false;
    RemoveDir(Elements[ne].DirNumber);
	ReBuild();
	return true;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::RemoveElement(int n)
{
	Elements[n]=Elements[NumElements-1];
	NumElements--;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::ReBuild()
{
	FILE *tf=tmpfile();
	Header.NumElements=NumElements;
	fseek(fh,0,SEEK_SET);
	fwrite(&Header,sizeof(Header),1,tf);
	for(int n=0;n<NumElements;n++)
	{
		fseek(fh,Elements[n].FileOffset-sizeof(NukeFile_ElmHeader),SEEK_SET);
		CopyData(fh,tf,Elements[n].FileSize+sizeof(NukeFile_ElmHeader));
	}
	fseek(tf,0,SEEK_SET);
	fclose(fh);
	fh=fopen(CurrentFileName,"w+b");
	CopyData(tf,fh,FileSize(tf));
	fclose(tf);
	ReadFile();
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::RemoveDir(int DirNumber)
{
	int n;
	ScanAgain:
	for(n=0;n<NumElements;n++)
	{
		if(Elements[n].DirPos==DirNumber)
		{
			if(Elements[n].Type==NFELMTYPE_DIR)
			{
				RemoveDir(Elements[n].DirNumber);
			}else if(Elements[n].Type==NFELMTYPE_FILE)
			{
				RemoveElement(n);
			}
			goto ScanAgain;
		}
	}
	for(n=0;n<NumElements;n++)
	{
		if(Elements[n].DirNumber==DirNumber&&Elements[n].Type==NFELMTYPE_DIR)
		{
			RemoveElement(n);
			return true;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::Rename(LPSTR Name,LPSTR NewName)
{
	if(!FileOpen)return false;

	char TempName[NF_NAMESIZE];
	char FullNewName[NF_NAMESIZE];
	if(SplitNameAndDir(Name,TempName,FullNewName))
	{
		strcat(FullNewName,"/");
		strcat(FullNewName,NewName);
		if(Find(FullNewName)>=0)return false;
	}else if(Find(NewName)>=0)return false;

	int ne=Find(Name);
	if(ne<0)return false;
	strcpy(Elements[ne].Name,NewName);
	WriteElmHeader(ne);
	return true;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::WriteElmHeader(int n)
{
	fseek(fh,Elements[n].FileOffset-sizeof(NukeFile_ElmHeader),SEEK_SET);
	fwrite(&Elements[n],sizeof(NukeFile_ElmHeader),1,fh);
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::ScanTreeEx(int DirNumber, NF_SCANCALLBACK CallBack)
{
	char Dir[NF_NAMESIZE];
	char FileName[NF_NAMESIZE];
	char FullPath[NF_NAMESIZE];
	int n;
	for(n=0;n<NumElements;n++)
	{
		if(Elements[n].DirPos==DirNumber&&(Elements[n].Type==NFELMTYPE_FILE))
		{
			GetFullPath(n,FullPath);
			SplitNameAndDir(FullPath,FileName,Dir);
			CallBack(&Elements[n],FullPath);
		}
	}
	for(n=0;n<NumElements;n++)
	{
		if(Elements[n].DirPos==DirNumber&&Elements[n].Type==NFELMTYPE_DIR)
		{
			GetFullPath(n,FullPath);
			SplitNameAndDir(FullPath,FileName,Dir);
			CallBack(&Elements[n],FullPath);
			ScanTreeEx(Elements[n].DirNumber,CallBack);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::Move(LPSTR Name, LPSTR Destination)
{
	int DestDir=0;
	if(Name==NULL)return false;
	if(Destination!=NULL)
	{
		DestDir=FindDirNumber(Destination);
		if(DestDir<0)return false;
	}
	int fn=Find(Name);
	if(fn>=0)
	{
		Elements[fn].DirPos=DestDir;
		WriteElmHeader(fn);
		return true;
	}else return false;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::AddDir(LPSTR sdir)
{
	//CreateDir(sdir);
	AddDir("",sdir,1);
	return 1;
}

bool CCab::AddDir(LPSTR Dest, LPSTR dirname, bool SubDirs)
{
	int n;
	char DirName[NF_NAMESIZE];
	if(Dest!=NULL)
	{
		strcpy(DirName,Dest);
		if(Dest[0]!=0)strcat(DirName,"/");
	}else DirName[0]=0;
	for(n=strlen(dirname)-1;n>=0;n--)
	{
		if(CheckSlash(dirname[n]))break;
	}
	strcat(DirName,&dirname[n+1]);
	if(!CreateDir(DirName))return false;
	return AddDirEx(DirName,dirname,SubDirs);
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::AddDirEx(LPSTR Dest, LPSTR dirname, bool SubDirs)
{
	char IndexName[NF_NAMESIZE];
	char FileName[NF_NAMESIZE];
	char DirName[NF_NAMESIZE];
	strcpy(DirName,dirname);
	if(strlen(DirName)>0&&!CheckSlash(DirName[strlen(DirName)-1]))strcat(DirName,"\\");
	if(Dest!=NULL&&Dest[0]==0)Dest=NULL;

	CDirScanner DirScanner;

	DirScanner.Start(DirName);

	while(DirScanner.GetFile())
	{
		if(DirScanner.Error())return false;
		if(Dest!=NULL)
		{
			strcpy(IndexName,Dest);
			strcat(IndexName,"/");
		}
		else IndexName[0]=0;

		strcat(IndexName,DirScanner.FindData.cFileName);

		if(DirScanner.FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(SubDirs)
			{
				if(!CreateDir(IndexName))return false;
				strcpy(FileName,DirName);
				strcat(FileName,DirScanner.FindData.cFileName);
                if(!AddDirEx(IndexName,FileName,true))return false;
			}
		}else
		{
			strcpy(FileName,DirName);
			strcat(FileName,DirScanner.FindData.cFileName);
            if(!AddFile(IndexName,FileName))return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////



int CCab::GetFileSize(LPSTR Name)
{
	int nf=FindFile(Name);
	if(nf<0) return -1;
	return Elements[nf].UncompSize;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::GetCompressedFileSize(LPSTR Name)
{
	int nf=FindFile(Name);
	if(nf<0)return -1;
	return Elements[nf].FileSize;
}

//////////////////////////////////////////////////////////////////////////////

// Set _bWriteHeader = true to write the header immediately, defaults to false.

bool CCab::SetFileDescription(LPSTR Desc, bool _bWriteHeader)
{
	memset(FileDesc,0,256);
    // Vars and Assignment
	if(Desc==NULL||strlen(Desc)>255)return false;
	strcpy(FileDesc,Desc);

    // Write the header immediately.
    if ( _bWriteHeader )
    {
        ZeroMemory(Header.Description,sizeof(Header.Description));
        strcpy(Header.Description, FileDesc);
        if ( !WriteHeader() )
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::Find(LPSTR Name)
{
	if(Name==NULL)return -1;
	char FullName[NF_NAMESIZE];
	for(int n=0;n<NumElements;n++)
	{
		GetFullPath(n,FullName);
		if(strcmpi(FullName,Name)==0)return n;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::FindDir(LPSTR Name)
{
	if(Name==NULL)return -1;
	char FullName[NF_NAMESIZE];
	for(int n=0;n<NumElements;n++)
	{
		if(Elements[n].Type==NFELMTYPE_DIR)
		{
			GetFullPath(n,FullName);
			if(strcmpi(FullName,Name)==0)return n;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::FindFile(LPSTR Name)
{
	if(Name==NULL)
		return -1;

	//Log("%s",Name);

	char FullName[NF_NAMESIZE];
	for(int n=0;n<NumElements;n++)
	{
		if(Elements[n].Type==NFELMTYPE_FILE)
		{
			GetFullPath(n,FullName);

			//Log("%s %s",Name,FullName);

			if(strcmpi(FullName,Name)==0) return n;

		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::FindDirNumber(LPSTR Name)
{
	if(Name==NULL)return 0;
	if(strcmpi(Name,"")==0)return 0;
	int dn=FindDir(Name);
	if(dn>=0)return Elements[dn].DirNumber;
	return -1;
}

//////////////////////////////////////////////////////////////////////////////

int CCab::GetFullLength()
{
	int l=sizeof(NukeFile_Header);
	for(int n=0;n<NumElements;n++)
	{
		l+=sizeof(NukeFile_ElmHeader);
		if(Elements[n].Type==NFELMTYPE_FILE)l+=Elements[n].FileSize;
	}
	return l;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::AddFile_Compress(LPSTR Name, LPSTR filename)
{
	FILE *SourceFile=fopen(filename,"rb");
	if(SourceFile==NULL)return false;
	int fs=FileSize(SourceFile);
	unsigned long destLen=(unsigned long)(fs*1.01+12); // Just to be safe take 1% more room. (The ZLib doc says 0.1%)
	unsigned char *SourceBuffer=(unsigned char *)malloc(fs);
	unsigned char *DestBuffer=(unsigned char *)malloc(destLen);
	fread(SourceBuffer,fs,1,SourceFile);
	compress2(DestBuffer,&destLen,SourceBuffer,fs,CompLevel);
	fclose(SourceFile);
    if(!CreateCompFile(Name,destLen,fs,CompLevel))
    {
		free(SourceBuffer);
        free(DestBuffer);
        return false;
    }
	FILE *DestFile=Seek(Name);
    if(DestFile==NULL)
    {
        free(SourceBuffer);
        free(DestBuffer);
        return false;
    }
	fwrite(DestBuffer,destLen,1,DestFile);
    free(SourceBuffer);
    free(DestBuffer);
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::ExtractFile_ToMem(LPSTR Name, void *dest)
{
	int ElmNum=FindFile(Name);
	if(ElmNum<0)return false;
	FILE *SourceFile=Seek(Name);
	if(Elements[ElmNum].CompressLevel==NFCOMP_NONE)
	{
		fread(dest,Elements[ElmNum].UncompSize,1,SourceFile);
	}
	else
	{
		void *CompData=malloc(Elements[ElmNum].FileSize);
		if(CompData==NULL)return false;
		fread(CompData,Elements[ElmNum].FileSize,1,SourceFile);
		unsigned long destLen=Elements[ElmNum].UncompSize;
		uncompress((unsigned char*)dest,&destLen,(unsigned char*)CompData,Elements[ElmNum].FileSize);
		free(CompData);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::ExtractFile_ToFile(LPSTR Name, LPSTR FileName)
{
	int fn=FindFile(Name);
	if(fn<0)return false;
	int fs=GetFileSize(Name);
	void *dest=malloc(fs);
	ExtractFile_ToMem(Name,dest);
	FILE *DestFile=fopen(FileName,"wb");
	fwrite(dest,fs,1,DestFile);
	fclose(DestFile);
	free(dest);
	return true;
}

//////////////////////////////////////////////////////////////////////////////

void CCab::SelectCompression(DWORD Level)
{
	CompLevel=Level;
}

//////////////////////////////////////////////////////////////////////////////

NF_FileBuffer CCab::GetFile(LPSTR Name)
{
	NF_FileBuffer FileBuffer;
	FileBuffer.Size=GetFileSize(Name);
	FileBuffer.FileBuffer=(unsigned char*)malloc(FileBuffer.Size);
	ExtractFile_ToMem(Name,FileBuffer.FileBuffer);
	return FileBuffer;
}

//////////////////////////////////////////////////////////////////////////////

bool CCab::ChangeCompression(LPSTR Name, DWORD clevel)
{
	int fn=FindFile(Name);
	if(fn<0)return false;
	if(Elements[fn].CompressLevel==clevel)return true;
	int SourceSize=Elements[fn].UncompSize;
	unsigned char *SourceBuffer=(unsigned char*)malloc(SourceSize);
	ExtractFile_ToMem(Name,SourceBuffer);
	RemoveFile(Name);

	if(clevel==NFCOMP_NONE)
	{
		CreateFile(Name,SourceSize);
		Seek(Name);
		fwrite(SourceBuffer,SourceSize,1,fh);
	}else
	{
		unsigned long destLen=(unsigned long)(SourceSize*1.01+12);
		unsigned char *DestBuffer=(unsigned char *)malloc(destLen);
		compress2(DestBuffer,&destLen,SourceBuffer,SourceSize,clevel);
		CreateCompFile(Name,destLen,SourceSize,clevel);
		Seek(Name);
		fwrite(DestBuffer,destLen,1,fh);
		free(DestBuffer);
	}
	free(SourceBuffer);
	return true;
}

//////////////////////////////////////////////////////////////////////////////

NukeFile_ElmHeader *CCab::GetFileInfo(LPSTR Name)
{
    int nf=FindFile(Name);
    if(nf<0)return NULL;
    return &Elements[nf];
}

//////////////////////////////////////////////////////////////////////////////

// Remove Many files...
// Allows removal of many files without rebuilding the nuk file every time.
// NF.ManyFileRemove( "OldFile.Txt" );
// NF.ManyFileRemove( "Graphics/SpaceShip.bmp" );
// NF.ManyFileEnd(); // MUST Call this!!
// Call once for each file.

bool CCab::ManyFileRemove ( LPSTR Name )
{
    if(!FileOpen)return false;
    int ne=FindFile(Name);
    if(ne==-1)return false;
    RemoveElement ( ne );
    return true;
}

//////////////////////////////////////////////////////////////////////////////
// End the ManyFileRemove process

bool CCab::ManyFileEnd ( void )
{
    if(!FileOpen)return false;
    ReBuild();
    return true;
}
