#include <NukeFile.h>

CNukeFile::CDirScanner::CDirScanner()
{	
	Handle=INVALID_HANDLE_VALUE;
}

CNukeFile::CDirScanner::~CDirScanner()
{
	if(Handle!=INVALID_HANDLE_VALUE)FindClose(Handle);
}

void CNukeFile::CDirScanner::Start(LPSTR dirname)
{
	strcpy(DirName,dirname);
	strcat(DirName,"*.*");
	NextIsFirst=true;
	GetFile();
	GetFile();
}

bool CNukeFile::CDirScanner::GetFile()
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

bool CNukeFile::CDirScanner::Error()
{
	if(Handle==INVALID_HANDLE_VALUE)return true;
	return false;
}
