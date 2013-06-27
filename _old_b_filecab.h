/* Seth's File Cabinet Class */

#ifndef SETH_FILE_CAB
#define SETH_FILE_CAB

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dlstorm.h"
#include <zLib.h>

// Maximum size of Indexname
#define NF_NAMESIZE 256

// Element types.
#define NFELMTYPE_FILE		0	// a file.
#define NFELMTYPE_DIR		1	// a directory.

// Compression modes
#define NFCOMP_NONE			Z_NO_COMPRESSION		// Default
#define NFCOMP_FORSPEED		Z_BEST_SPEED
#define NFCOMP_NORMAL		Z_DEFAULT_COMPRESSION
#define NFCOMP_BEST			Z_BEST_COMPRESSION

struct NukeFile_Header;
struct NukeFile_ElmHeader;
class  CCab;

typedef void (*NF_SCANCALLBACK)(NukeFile_ElmHeader *ElmInfo,LPSTR FullPath);

#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;}
#define DEL(x) if(x!=NULL){delete x;x=NULL;}
#define FREE(x) if(x!=NULL){free(x);x=NULL;}

struct NukeFile_Header
{
	DWORD Size;			// Size of this header
	DWORD Version;		// Version
	char Description[256];	// Description
	int NumElements;	// Amount of elements
};

struct NukeFile_ElmHeader
{
	DWORD	FileSize;	// Size of this file.
	DWORD	FileOffset;	// This files contents Offset in the nukefile.
	DWORD	Type;		// Type of this element. (Dir or File)
	DWORD	CompressLevel; // Level of compression.
	char	Name[NF_NAMESIZE];	// Name of this file/directory.
	int		DirPos;		// DirPos is what directory this belongs to.
	int		DirNumber;	// DirNumber is what directory this is (if it's a dir).
	int		UncompSize;	// Uncompressed size of compressed files.
};

#ifndef NF_FILEBUF_DEFINED
#define NF_FILEBUF_DEFINED
struct NF_FileBuffer
{
	unsigned char *FileBuffer;
	int Size;
};
#endif

class CCab
{
public:


    // Remove Many files...
    // Allows removal of many files without rebuilding the nuk every time.
    // Calling ManyFileEnd is obligatory!!
    // Use like so:
    // NF.ManyFileRemove( "OldFile.Txt" );
    // NF.ManyFileRemove( "Graphics/SpaceShip.bmp" );
    // NF.ManyFileEnd();
    bool ManyFileRemove ( LPSTR Name ); // Call once for each file.
    bool ManyFileEnd ( void );          // End the ManyFileRemove (You MUST call this!)

	// Define the description in the Header of the NUK file.
	// 0 to 255 chars in size.
    bool SetFileDescription(LPSTR Desc, bool _bWriteHeader = false);

	// Return a pointer to the NukeFile_ElmHeader structure describing 'Name'.
	// Returns NULL if file doesn't exists.
	NukeFile_ElmHeader *GetFileInfo(LPSTR Name);

	// Change compression-level of a file.
	bool ChangeCompression(LPSTR Name,DWORD clevel);

	// Allocate memory, and extract a file into it.
	NF_FileBuffer GetFile(LPSTR Name);

	// Select between the 4 compression modes. (NFCOMP_xxx)
	// All new files will have this compression-level.
    void SelectCompression(DWORD Level);

    // Returns the current default compression level. One of the #defines NFCOMP_****
    DWORD GetCompressionLevel ( void )  { return CompLevel; }

    // RetVal = true if the This CNukeFile object has an open .NUK file.
    bool IsFileOpen ( void )            { return FileOpen; }

    // Returns the .NUK file's Description (What Builder `NUK File Header (255 Chars max)`
    LPCSTR GetDescriptionString ( void ) { return *&Header.Description; }

	// Add a file, with the IndexName 'Name'.
	bool AddFile(LPSTR Name,LPSTR filename);
    bool AddFileFromMem(LPSTR Name,unsigned char *fb,int size);

	// Extract a file to file. If the file is compressed, this function will uncompress it.
	// ect. ExtractFile_ToFile("Graphics/SpaceShip.bmp","C:\\SpaceShip.bmp");
	bool ExtractFile_ToFile(LPSTR Name,LPSTR FileName);

	// Extract a file to memory. If the file is compressed, this function will uncompress it.
	// ect.
	// void Buffer=malloc(GetFileSize(Name));
	// ExtractFile_ToMem(Name,Buffer);
	bool ExtractFile_ToMem(LPSTR Name,void *dest);

	// Get the size of a file.
	// If file is compressed, this functions returns the uncompressed size.
	int GetFileSize(LPSTR Name);

	// Get the size of a file.
	// If file is compressed, this functions returns the compressed size.
	int GetCompressedFileSize(LPSTR Name);

    // -------------------------------------------------------------------
    // Add all files and directories (if SubDirs=true).
	// ect. AddDir("TestDir","C:\\stuff",true);
    // LPSTR Dest                       The Name of destination dir in the nuk
    //                                      file.  This can be "" or NULL to
    //                                      place dirname at the root of the
    //                                      nuk file.
    //										Please note, Dest MUST already exist in
    //                                      the nuk file!
    // LPSTR dirname						The location of the source folder on
    //                                      your system.
    //										Omit any trailing '\\' characters. i.e.
    //                                      "C:\\stuff" NOT "C:\\stuff\\"
    // bool SubDirs							true to add SubFolders from `dirname`
    //                                      also, else just the contents of
    //                                      `dirname`

	bool AddDir(LPSTR sdir);

	bool AddDir(LPSTR Dest,LPSTR dirname,bool SubDirs);

	// Move a file or a directory + content
	bool Move(LPSTR Name,LPSTR Destination);

	// Rename a file or a directory
	// ect. Rename("Testdir/image.bmp","stuff.bmp");
	bool Rename(LPSTR Name,LPSTR NewName);

	// Remove a directory and all it's content
	bool RemoveDir(LPSTR Name);

	// Removes a file.
	bool RemoveFile(LPSTR Name);

	// Scan Directory - Calls 'CallBack' with this information about the elements in the dir
	bool ScanDir(LPSTR Name,NF_SCANCALLBACK CallBack);

	// Uses ScanDir to scan this dir and all subdirs, for elements.
	bool ScanTree(LPSTR Name,NF_SCANCALLBACK CallBack);

	// Seek to the position of the file 'Name' and return a filehandle. Returns NULL on error.
	FILE * Seek(LPSTR Name);

	// Creates a dir.
	bool CreateDir(LPSTR Name);

	// Create an empty file, with the size 'Size'. (ect. CreateFile("Image.raw",ImageWidth*ImageHeight); )
	bool CreateFile(LPSTR Name, DWORD Size);

	// Close the current file.
	bool Close();

	// Create a new, and empty, NukeFile.
	bool Create(LPSTR fn);

	// Open an existing NukeFile.
	// The FileDescription in the NukeFile must match the specified filedescripion.
	// (Use SetFileDescription)
    // `_bIgnoreDescription = true` to ignore the NUK file's description.
    bool Open ( LPSTR fn, bool _bIgnoreDescription = false );

	CCab();
	virtual ~CCab();

private:
	bool AddFile_Compress(LPSTR Name,LPSTR filename);
    bool CreateCompFile(LPSTR Name, DWORD CompSize, DWORD Size, DWORD clevel);
	DWORD CompLevel;
	int GetFullLength();
	class CDirScanner
	{
	public:
		bool Error();
		char DirName[NF_NAMESIZE];
		WIN32_FIND_DATA FindData;
		HANDLE Handle;
		bool GetFile();
		bool NextIsFirst;
		void Start(LPSTR dirname);
		CDirScanner();
		virtual ~CDirScanner();
	};
	int FindDirNumber(LPSTR Name);
	int FindDir(LPSTR Name);
	int FindFile(LPSTR Name);
	int Find(LPSTR Name);
    bool AddDirEx(LPSTR Dest,LPSTR dirname,bool SubDirs);
	char FileDesc[256];
	bool ScanTreeEx(int DirNumber,NF_SCANCALLBACK CallBack);
	bool RemoveDir(int DirNumber);
	void WriteElmHeader(int n);
	void ReBuild();
	void RemoveElement(int n);
    bool ReadFile(bool _bIgnoreDescription = false );
	int NumElements;
	NukeFile_ElmHeader *Elements;
	NukeFile_Header Header;
	bool SplitNameAndDir(LPSTR Source,LPSTR FileName,LPSTR Dir);
	bool GetFullPath(int n,LPSTR Dest);
	int TempDir;
	bool CheckSlash(char c);
	int GetNumSlashes(LPSTR String);
	int GetUntilSlash(LPSTR Source,LPSTR Dest);
	void CopyData(FILE *Source,FILE *Dest,int Size);
	bool WriteHeader();
	int FileSize(FILE *f);
	int FindAvailDir();
	void AddElement(NukeFile_ElmHeader *Element);
	int MaxElements;
	FILE *fh;
	char CurrentFileName[256];
	bool FileOpen;
	void SetAmount(int a);
};

//////////////////////////////////////////////////////////////////////////
//  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //
//////////////////////////////////////////////////////////////////////////
#endif //#ifndef macHeader_NukeFile_h
//
///EOF
