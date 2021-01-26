// filesys.cc
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "filesys.h"

//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{
  DEBUG('f', "Initializing the file system.\n");
  if (format)
  {
    BitMap *freeMap = new BitMap(NumSectors);
    Directory *directory = new Directory(NumDirEntries);
    FileHeader *mapHdr = new FileHeader;
    FileHeader *dirHdr = new FileHeader;

    DEBUG('f', "Formatting the file system.\n");

    // First, allocate space for FileHeaders for the directory and bitmap
    // (make sure no one else grabs these!)
    freeMap->Mark(FreeMapSector);
    freeMap->Mark(DirectorySector);

    // Second, allocate space for the data blocks containing the contents
    // of the directory and bitmap files.  There better be enough space!

    ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
    ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));

    // Flush the bitmap and directory FileHeaders back to disk
    // We need to do this before we can "Open" the file, since open
    // reads the file header off of disk (and currently the disk has garbage
    // on it!).

    DEBUG('f', "Writing headers back to disk.\n");
    mapHdr->WriteBack(FreeMapSector);
    dirHdr->WriteBack(DirectorySector);

    // OK to open the bitmap and directory files now
    // The file system operations assume these two files are left open
    // while Nachos is running.

    freeMapFile = new OpenFile(FreeMapSector);
    directoryFile = new OpenFile(DirectorySector);
    currentDirFile = directoryFile;

    // Once we have the files "open", we can write the initial version
    // of each file back to disk.  The directory at this point is completely
    // empty; but the bitmap has been changed to reflect the fact that
    // sectors on the disk have been allocated for the file headers and
    // to hold the file data for the directory and bitmap.

    directory->AddDir(".", DirectorySector);
    directory->AddDir("..", DirectorySector); //TODO : je suggère de virer ca, le repertoire racine
    //n'en a pas besoin et si on veut eventuellement avoir des absolut path, cela risquerait de poser
    //problème. Et ca permet de faire des cd ../../../../../.. .... à l'infini.

    DEBUG('f', "Writing bitmap and directory back to disk.\n");
    freeMap->WriteBack(freeMapFile); // flush changes to disk
    directory->WriteBack(directoryFile);

    if (DebugIsEnabled('f'))
    {
      freeMap->Print();
      directory->Print();

      delete freeMap;
      delete directory;
      delete mapHdr;
      delete dirHdr;
    }
  }
  else
  {
    // if we are not formatting the disk, just open the files representing
    // the bitmap and directory; these are left open while Nachos is running
    freeMapFile = new OpenFile(FreeMapSector);
    directoryFile = new OpenFile(DirectorySector);
    currentDirFile = directoryFile;
  }
  openFiles = new FileTable(NbOpenedFiles);
}

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name or the path of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool FileSystem::Create(const char *name, int initialSize)
{
  OpenFile *currentDirFileSave = currentDirFile;
  //Try to navigate to last repertory before file we want to create
  char *rep = (char *)malloc(sizeof(char) * FileNameMaxLen);
  if (!navigateToPath(name, rep))
  {
    currentDirFile = currentDirFileSave;
    free(rep);
    return FALSE;
  }

  Directory *directory;
  BitMap *freeMap;
  FileHeader *hdr;
  int sector;
  bool success;

  DEBUG('f', "Creating file %s, size %d\n", name, initialSize);

  directory = new Directory(NumDirEntries);
  directory->FetchFrom(currentDirFile);

  if (directory->Find(rep) != -1)
    success = FALSE; // file is already in directory
  else
  {
    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);
    sector = freeMap->Find(); // find a sector to hold the file header
    if (sector == -1)
      success = FALSE; // no free block for file header
    else if (!directory->Add(rep, sector))
      success = FALSE; // no space in directory
    else
    {
      hdr = new FileHeader;
      if (!hdr->Allocate(freeMap, initialSize))
        success = FALSE; // no space on disk for data
      else
      {
        success = TRUE;
        // everthing worked, flush all changes back to disk
        hdr->WriteBack(sector);
        directory->WriteBack(currentDirFile);
        freeMap->WriteBack(freeMapFile);
      }
      delete hdr;
    }
    delete freeMap;
  }
  delete directory;
  currentDirFile = currentDirFileSave;
  free(rep);
  return success;
}

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.
//	To open a file:
//	  Find the location of the file's header, using the directory
//	  Bring the header into memory
//
//	"name" -- the text name or the path of the file to be opened
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(const char *name)
{
  OpenFile *currentDirFileSave = currentDirFile;
  //Try to navigate to last repertory before repertory we want to create
  char *rep = (char *)malloc(sizeof(char) * FileNameMaxLen);
  if (!navigateToPath(name, rep))
  {
    currentDirFile = currentDirFileSave;
    free(rep);
    return NULL;
  }

  Directory *directory = new Directory(NumDirEntries);
  OpenFile *openFile = NULL;
  int sector;

  DEBUG('f', "Opening file %s\n", rep);
  directory->FetchFrom(currentDirFile);
  sector = directory->Find(rep);
  if (sector >= 0){

   /*openFile = currentThread->getFileTable()->getFile(sector);
    if(openFile == NULL){
        return NULL;
    }*/
    openFile = openFiles->getOrCreateOpenFile(sector);
  }
  
  currentDirFile = currentDirFileSave;
  free(rep);
  delete directory;
  return openFile; // return NULL if not found
}

/**
 * Close, if exists, a given file from given relative path.
 * 
 * @param name, file's relative path.
 * @return true if file has been close, else otherwise.
 */
bool FileSystem::Close(OpenFile *file)
{
  if (openFiles->RemoveFile(file))
    delete file;
  return TRUE;
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name or the path of the file to be removed
//----------------------------------------------------------------------

bool FileSystem::Remove(const char *name)
{
  OpenFile *currentDirFileSave = currentDirFile;
  //Try to navigate to last repertory before repertory we want to create
  char *rep = (char *)malloc(sizeof(char) * FileNameMaxLen);
  if (!navigateToPath(name, rep))
  {
    currentDirFile = currentDirFileSave;
    free(rep);
    return FALSE;
  }

  Directory *directory;
  BitMap *freeMap;
  FileHeader *fileHdr;
  int sector;

  directory = new Directory(NumDirEntries);
  directory->FetchFrom(currentDirFile);
  sector = directory->Find(rep);
  if (sector == -1)
  {
    free(rep);
    currentDirFile = currentDirFileSave;
    delete directory;
    return FALSE; // file not found
  }
  fileHdr = new FileHeader;
  fileHdr->FetchFrom(sector);

  freeMap = new BitMap(NumSectors);
  freeMap->FetchFrom(freeMapFile);

  fileHdr->Deallocate(freeMap); // remove data blocks
  freeMap->Clear(sector);       // remove header block
  directory->Remove(rep);

  freeMap->WriteBack(freeMapFile);      // flush to disk
  directory->WriteBack(currentDirFile); // flush to disk
  delete fileHdr;
  delete directory;
  delete freeMap;
  currentDirFile = currentDirFileSave;
  free(rep);
  return TRUE;
}

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
bool FileSystem::navigateToPath(const char *name, char *rep)
{
  int i = -1;
  int j = 0;
  char c = name[i];
  do
  {
    i++;
    c = name[i];
    j = 0;
    //Retrieve first repertory in path (before the first /)
    while (c != '/' && c != '\n' && c != '\0' && j < FileNameMaxLen)
    {
      rep[j] = name[i];
      i++;
      j++;
      c = name[i];
    }
    rep[j] = '\0';

    if (c == '/')
      if (!OpendDir(rep))
      {
        return false;
      }
  } while (c == '/');
  return true;
}

/**
 * Create a new Directory from given relative path.
 * 
 * @param name, directory's relative path to create.
 * @return true if directory has been created, false otherwise.
 */
bool FileSystem::CreateDir(const char *name)
{
  OpenFile *currentDirFileSave = currentDirFile;

  //Try to navigate to last repertory before repertory we want to create
  char *rep = (char *)malloc(sizeof(char) * FileNameMaxLen);
  if (!navigateToPath(name, rep))
  {
    currentDirFile = currentDirFileSave;
    free(rep);
    return FALSE;
  }

  Directory *dir_child;

  BitMap *freeMap;
  FileHeader *hdr;
  OpenFile *childFile;

  int sector_parent;
  bool success;
  int sector;

  Directory *directory;
  directory = new Directory(NumDirEntries);
  directory->FetchFrom(currentDirFile);
  DEBUG('f', "Creating directory %s\n", rep);

  if (directory->Find(rep) != -1)
    success = FALSE; // file is already in directory

  else
  {
    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);
    sector = freeMap->Find(); // find a sector to hold the file header

    if (sector == -1)
      success = FALSE; // no free block for file header
    else if (!directory->AddDir(rep, sector))
      success = FALSE; // no space in directory

    else
    {
      hdr = new FileHeader;

      dir_child = new Directory(NumDirEntries);
      dir_child->AddDir(".", sector);

      sector_parent = directory->FindDir(".");

      if (sector_parent != -1)
        dir_child->AddDir("..", sector_parent);

      if (DebugIsEnabled('f'))
      {
        DEBUG('f', "Liste du nouveau repertoire :\n");
        dir_child->List();
      }

      if (!hdr->Allocate(freeMap, DirectoryFileSize))
      {
        success = FALSE; // no space on disk for data
      }
      else
      {
        success = TRUE;

        hdr->WriteBack(sector);
        childFile = new OpenFile(sector);
        // everthing worked, flush all changes back to disk

        dir_child->WriteBack(childFile);
        directory->WriteBack(currentDirFile);
        freeMap->WriteBack(freeMapFile);
        DEBUG('f', "New directory created : success\n");
        delete childFile;
      }
      delete hdr;
      delete dir_child;
    }
    delete directory;
  }
  currentDirFile = currentDirFileSave;
  free(rep);
  return success;
}
/**
 * Try to move current directory to given dircetory name.
 * 
 * @param name directory to open.
 */
bool FileSystem::OpendDir(const char *name)
{
  if (strcmp(name,".") == 0)
    return true;

  //try to find the first directory in path
  Directory *directory = new Directory(NumDirEntries);
  directory->FetchFrom(currentDirFile);
  int sector = directory->FindDir(name);
  delete directory;
  DEBUG('f', "Search a sector for change current dir\n");

  //If first directory isn't found, return false
  if (sector == -1)
    return false;

  //Move to first directory in path
  currentDirFile = new OpenFile(sector);
  DEBUG('f', "We change for %s directory\n", name);
  return true;
}

/**
 * Move from current directory to name, which could be a relative path.
 * 
 * @param name relative path to the directory to reach.
 * @return true if directory has been reached, false otherwise.
 */
bool FileSystem::ChangeDir(const char *name)
{
  OpenFile *currentDirFileSave = currentDirFile;

  bool success = TRUE;

  char *rep = (char *)malloc(sizeof(char) * FileNameMaxLen);
  if (!navigateToPath(name, rep))
    success = FALSE;

  if (success)
    success = OpendDir(rep);

  free(rep);
  if (!success)
    currentDirFile = currentDirFileSave;

  return success;
}

/**
 * Remove, if exists, a directory from from given relative path.
 * 
 * @param name relative path to the directory to remove.
 * @return true if directory has been created, false otherwise.
 */
bool FileSystem::RemoveDir(const char *name)
{
  OpenFile *currentDirFileSave = currentDirFile;

  //Try to navigate to last repertory before repertory we want to create
  char *rep = (char *)malloc(sizeof(char) * FileNameMaxLen);
  if (!navigateToPath(name, rep))
  {
    currentDirFile = currentDirFileSave;
    free(rep);
    return FALSE;
  }

  Directory *directory = new Directory(NumDirEntries);
  Directory *dir_child = new Directory(NumDirEntries);

  directory->FetchFrom(currentDirFile);

  int sector = directory->FindDir(rep);
  if (sector == -1)
  {
    currentDirFile = currentDirFileSave;
    free(rep);
    return FALSE;
  }

  OpenFile *dir_child_file = new OpenFile(sector);

  dir_child->FetchFrom(dir_child_file);

  bool empty = dir_child->isEmpty();

  delete dir_child;
  delete dir_child_file;

  if (empty)
  {
    directory->Remove(rep);
    directory->WriteBack(currentDirFile);
    DEBUG('f', "We remove %s directory\n", rep);
    delete directory;
    currentDirFile = currentDirFileSave;
    free(rep);
    return TRUE;
  }
  delete directory;
  currentDirFile = currentDirFileSave;
  free(rep);
  return FALSE;
}


//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------

void FileSystem::List()
{
  Directory *directory = new Directory(NumDirEntries);

  directory->FetchFrom(currentDirFile);
  directory->List();
  delete directory;
}

//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

void FileSystem::Print()
{
  FileHeader *bitHdr = new FileHeader;
  FileHeader *dirHdr = new FileHeader;
  BitMap *freeMap = new BitMap(NumSectors);
  Directory *directory = new Directory(NumDirEntries);

  printf("Bit map file header:\n");
  bitHdr->FetchFrom(FreeMapSector);
  bitHdr->Print();

  printf("Directory file header:\n");
  dirHdr->FetchFrom(DirectorySector);
  dirHdr->Print();

  freeMap->FetchFrom(freeMapFile);
  freeMap->Print();

  directory->FetchFrom(currentDirFile);
  directory->Print();

  delete bitHdr;
  delete dirHdr;
  delete freeMap;
  delete directory;
}

/**
 * 
 */
int FileSystem::getSector(OpenFile* openFile) 
{
  return openFiles->getSector(openFile);
}

/**
 * 
 */
OpenFile* FileSystem::getOpenFile(int sector)
{
  return openFiles->getFile(sector);
}