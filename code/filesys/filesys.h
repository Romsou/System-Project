// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.  This is provided in case the
//	multiprogramming and virtual memory assignments (which make use
//	of the file system) are done before the file system assignment.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "openfile.h"
#include "filetable.h"

// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known
// sectors, so that they can be located on boot-up.
#define FreeMapSector 0
#define DirectorySector 1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number
// of files that can be loaded onto the disk.
#define FreeMapFileSize (NumSectors / BitsInByte)

#define DirectoryFileSize (sizeof(DirectoryEntry) * NumDirEntries)

#define NbOpenedFiles 10

#ifdef FILESYS_STUB // Temporarily implement file system calls as 
										// calls to UNIX, until the real file system 
										// implementation is available
class FileSystem
{
public:
	FileSystem(bool format) {}

	bool Create(const char *name, int initialSize)
	{
		int fileDescriptor = OpenForWrite(name);

		if (fileDescriptor == -1)
			return FALSE;
		Close(fileDescriptor);
		return TRUE;
	}

	OpenFile *Open(char *name)
	{
		int fileDescriptor = OpenForReadWrite(name, FALSE);

		if (fileDescriptor == -1)
			return NULL;
		return new OpenFile(fileDescriptor);
	}

	bool Remove(char *name) { return Unlink(name) == 0; }
};

#else // FILESYS
class FileSystem
{
public:
	/**
	 * Initialize the file system. Must be called *after* "synchDisk" 
	 * has been initialized. If "format", there is nothing on
	 * the disk, so initialize the directory and the bitmap of free blocks.
	 * 
	 * @param format Boolean, true if we should format file system, false otherwise.
	 */
	FileSystem(bool format);

	/**
	 * Create a file (UNIX creat) from given relative path.
	 * 
	 * @param name, file's relative path.
	 * @param initialSize, file's size.
	 * @return true if file has been created, false otherwise.
	 */
	bool Create(const char *name, int initialSize);

	/**
	 * Open, if exists, a given file from given relative path.
	 * 
	 * @param name, file's relative path.
	 * @return an OpenFile pointer on the opened file.
	 */
	OpenFile *Open(const char *name); // Open a file (UNIX open)

	/**
	 * Close, if exists, a given file from given relative path.
	 * 
	 * @param name, file's relative path.
	 * @return true if file has been close, else otherwise.
	 */
	bool Close(OpenFile *file);

	/**
	 * Remove, if exists, a file from given relative path.
	 * 
	 * @param name, file's relative path.
	 * @return true if file has been deleted, false otherwise.
	 */
	bool Remove(const char *name); // Delete a file (UNIX unlink)

	/**
	 * Create a new Directory from given relative path.
	 * 
	 * @param name, directory's relative path to create.
	 * @return true if directory has been created, false otherwise.
	 */
	bool CreateDir(const char *name);

	/**
	 * Move from current directory to name, which could be a relative path.
	 * 
	 * @param name relative path to the directory to reach.
	 * @return true if directory has been reached, false otherwise.
	 */
	bool ChangeDir(const char *name);

	/**
	 * Remove, if exists, a directory from from given relative path.
	 * 
	 * @param name relative path to the directory to remove.
	 * @return true if directory has been created, false otherwise.
	 */
	bool RemoveDir(const char *name);

	/**
	 * List all the files in the file system.
	 */
	void List();

	/**
	 * List all the files and their contents
	 */
	void Print();

private:
	OpenFile *freeMapFile; // Bit map of free disk blocks,
			// represented as a file
	OpenFile *directoryFile; // "Root" directory -- list of
			// file names, represented as a file
	OpenFile *currentDirFile; // current directory

	FileTable *openFiles; //opened files array, len max = 10

	/**
	 * Try to move current directory on the last repertory before the last name
	 * (ie : ..../..../..../this one/.....) frome a given relative path name.
	 * Store in a given char* named rep the last part of the path.
	 * (ie: ..../..../..../...../this one). Be carefull, if return false, current
	 * directory could be anywhere in the path, we try to go as far as possible before the
	 * last part.
	 * 
	 * @param name relative path to the file/directory we should reach.
	 * @param rep a char* where we will store the last part of the path.
	 * @return true if we are now postionate on the last repertory before, otherwise
	 * return false.
	 */
	bool navigateToPath(const char *name, char *rep);

	/**
	 * Try to move current directory to a given directory name, if it exists. 
	 * (not handle any path)
	 * 
	 * @param name directory to open.
	 * @return true if we reach directory.
	 */
	bool OpendDir(const char *name);
};

#endif // FILESYS

#endif // FS_H
