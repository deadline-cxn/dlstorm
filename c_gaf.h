/***************************************************************
 **   DLSTORM   Deadline's Code Storm Library
 **          /\
 **   ---- D/L \----
 **       \/
 **   License:      BSD
 **   Copyright:    2020
 **   File:         c_gaf.h
 **   Class:        CGAF
 **   Description:  Game Archive File
 **                 Store game assets into one compressed file
 **                 Based on NukeDX Nukefile archiver
 **   Author:       Seth Parson
 **   Twitter:      @Sethcoder
 **   Website:      www.sethcoder.com
 **   Email:        defectiveseth@gmail.com
 **
 ***************************************************************/
#ifndef _DLCS_C_GAF_
#define _DLCS_C_GAF_
#include <dirent.h>
#include "dlstorm.h"
#include "zlib.h"
#include "c_log.h"
// Maximum size of a Indexname in CGAF File.
#define GAF_NAMESIZE 256
#define GAF_DESCSIZE 256
// Element types.
#define GAFELMTYPE_FILE		0	// a file.
#define GAFELMTYPE_DIR		1	// a directory.
// Compression modes
#define GAFCOMP_NONE		Z_NO_COMPRESSION		// Default
#define GAFCOMP_FORSPEED	Z_BEST_SPEED
#define GAFCOMP_NORMAL		Z_DEFAULT_COMPRESSION
#define GAFCOMP_BEST		Z_BEST_COMPRESSION
struct GAFFile_Header;
struct GAFFile_ElmHeader;
class  CGAF;
typedef void (*GAF_SCANCALLBACK)(GAFFile_ElmHeader *ElmInfo,const char * FullPath);
struct GAFFile_Header {
    DWORD Size;			// Size of this header
    DWORD Version;		// Version
    char Description[GAF_DESCSIZE];	// Description
    int NumElements;	// Amount of elements
};
struct GAFFile_ElmHeader {
    DWORD	FileSize;	// Size of this file.
    DWORD	FileOffset;	// This files contents Offset in the nukefile.
    DWORD	Type;		// Type of this element. (Dir or File)
    DWORD	CompressLevel; // Level of compression.
    char	Name[GAF_NAMESIZE];	// Name of this file/directory.
    int		DirPos;		// DirPos is what directory this belongs to.
    int		DirNumber;	// DirNumber is what directory this is (if it's a dir).
    int		UncompSize;	// Uncompressed size of compressed files.
};
#ifndef GAF_FILEBUF_DEFINED
#define GAF_FILEBUF_DEFINED
struct GAF_FileBuffer {
    unsigned char *fb;
    int Size;
};
#endif
///////////////////////// CGAF class
class CGAF {
public:

#ifdef GAFDEBUG
    CLog *CabLog;
#endif
    CGAF();
    CGAF(const char *file,int comp);
    virtual ~CGAF();

    // Remove Many files...
    // Allows removal of many files without rebuilding the nuk every time.
    // Calling ManyFileEnd is obligatory!!
    // Use like so:
    // GAF.ManyFileRemove( "OldFile.Txt" );
    // GAF.ManyFileRemove( "Graphics/SpaceShip.bmp" );
    // GAF.ManyFileEnd();
    bool ManyFileRemove ( const char * Name ); // Call once for each file.
    bool ManyFileEnd ( void );          // End the ManyFileRemove (You MUST call this!)

    // Define the description in the Header of the NUK file.
    // 0 to 255 chars in size.
    bool SetFileDescription(const char * Desc, bool _bWriteHeader = false);

    // Return a pointer to the NukeFile_ElmHeader structure describing 'Name'.
    // Returns NULL if file doesn't exists.
    GAFFile_ElmHeader *GetFileInfo(const char * Name);

    // Change compression-level of a file.
    bool ChangeCompression(const char * Name,DWORD clevel);

    // Allocate memory, and extract a file into it.
    GAF_FileBuffer GetFile(const char * Name);

    void GetFile(const char * Name, GAF_FileBuffer gfb);

    //void *GetSurface(char *fb);

    // Select between the 4 compression modes. (NFCOMP_xxx)
    // All new files will have this compression-level.
    void SelectCompression(DWORD Level);

    // Returns the current default compression level. One of the #defines NFCOMP_****
    DWORD GetCompressionLevel ( void )  {
        return CompLevel;
    }

    // RetVal = true if the This CNukeFile object has an open .NUK file.
    bool IsFileOpen ( void )            {
        return FileOpen;
    }

    // Returns the .NUK file's Description (What Builder `NUK File Header (255 Chars max)`
    LPCSTR GetDescriptionString ( void ) {
        return *&Header.Description;
    }

    // Add a file, with the IndexName 'Name'.
    bool AddFile(const char * Name,const char * filename);
    bool AddFileFromMem(const char * Name,unsigned char *fb,int size);

    // Extract a file to file. If the file is compressed, this function will uncompress it.
    // ect. ExtractFile_ToFile("Graphics/SpaceShip.bmp","C:\\SpaceShip.bmp");
    bool ExtractFile_ToFile(const char * Name,const char * FileName);

    // Extract a file to memory. If the file is compressed, this function will uncompress it.
    // ect.
    // void Buffer=malloc(GetFileSize(Name));
    // ExtractFile_ToMem(Name,Buffer);
    bool ExtractFile_ToMem(const char * Name,void *dest);

    // Get the size of a file.
    // If file is compressed, this functions returns the uncompressed size.
    int GetFileSize(const char * Name);

    // Get the size of a file.
    // If file is compressed, this functions returns the compressed size.
    int GetCompressedFileSize(const char * Name);

    // -------------------------------------------------------------------
    // Add all files and directories (if SubDirs=true).
    // ect. AddDir("TestDir","C:\\stuff",true);
    // char * Dest                       The Name of destination dir in the nuk
    //                                      file.  This can be "" or NULL to
    //                                      place dirname at the root of the
    //                                      nuk file.
    //										Please note, Dest MUST already exist in
    //                                      the nuk file!
    // char * dirname						The location of the source folder on
    //                                      your system.
    //										Omit any trailing '\\' characters. i.e.
    //                                      "C:\\stuff" NOT "C:\\stuff\\"
    // bool SubDirs							true to add SubFolders from `dirname`
    //                                      also, else just the contents of
    //                                      `dirname`
    bool AddDir(const char * Name);
    bool AddDir(const char * Dest,const char * dirname,bool SubDirs);
    bool AddDirFilesToRoot(const char * dir, bool SubDirs);

    // Move a file or a directory + content
    bool Move(const char * Name,const char * Destination);

    // Rename a file or a directory
    // ect. Rename("Testdir/image.bmp","stuff.bmp");
    bool Rename(const char * Name,const char * NewName);

    // Remove a directory and all it's content
    bool RemoveDir(const char * Name);

    // Removes a file.
    bool RemoveFile(const char * Name);

    // Scan Directory - Calls 'CallBack' with this information about the elements in the dir
    bool ScanDir(const char * Name,GAF_SCANCALLBACK CallBack);

    // Uses ScanDir to scan this dir and all subdirs, for elements.
    bool ScanTree(const char * Name,GAF_SCANCALLBACK CallBack);

    // Seek to the position of the file 'Name' and return a filehandle. Returns NULL on error.
    FILE * Seek(const char * Name);

    // Creates a dir.
    bool CreateDir(const char * Name);

    // Create an empty file, with the size 'Size'. (ect. CreateFile("Image.raw",ImageWidth*ImageHeight); )
    bool CreateFile(const char * Name, DWORD Size);

    // Close the current file.
    bool Close();

    // Create a new, and empty, NukeFile.
    bool Create(const char * fn);

    // Open an existing NukeFile.
    // The FileDescription in the NukeFile must match the specified filedescripion.
    // (Use SetFileDescription)
    // `_bIgnoreDescription = true` to ignore the NUK file's description.
    bool Open ( const char * fn, bool _bIgnoreDescription = false );
    bool AddFile_Compress(const char * Name,const char * filename);
    bool CreateCompFile(const char * Name, DWORD CompSize, DWORD Size, DWORD clevel);
    DWORD CompLevel;
    int GetFullLength();
    int FindDirNumber(const char * Name);
    int FindDir(const char * Name);
    int FindFile(const char * Name);
    int Find(const char * Name);
    bool AddDirEx(const char * Dest,const char * dirname,bool SubDirs);
    char FileDesc[GAF_DESCSIZE];
    bool ScanTreeEx(int DirNumber,GAF_SCANCALLBACK CallBack);
    bool RemoveDir(int DirNumber);
    void WriteElmHeader(int n);
    void ReBuild();
    void RemoveElement(int n);
    bool ReadFile(bool _bIgnoreDescription = false );
    int NumElements;
    GAFFile_ElmHeader *Elements;
    GAFFile_Header Header;
    bool SplitNameAndDir(const char * Source, char * FileName, char * Dir);
    bool GetFullPath(int n, char * Dest);
    int TempDir;
    bool CheckSlash(char c);
    int GetNumSlashes(const char * String);
    int GetUntilSlash(const char * Source,char * Dest);
    void CopyData(FILE *Source,FILE *Dest,int Size);
    bool WriteHeader();
    int FileSize(FILE *f);
    int FindAvailDir();
    void AddElement(GAFFile_ElmHeader *Element);
    int MaxElements;
    FILE *fh;
    char CurrentFileName[GAF_NAMESIZE];
    bool FileOpen;
    void SetAmount(int a);
};
//////////////////////////////////////////////////////////////////////////

#endif // _DLCS_C_GAF_

