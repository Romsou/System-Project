#include "filetable.h"

FileTable::FileTable(int nbMaxFiles)
{
  nbFiles = nbMaxFiles;
  openFiles = new fileOpen *[nbFiles];
  for (int i = 0; i < nbFiles; i++)
    openFiles[i] = NULL;

  filePresenceIndicator = new BitMap(nbFiles);
}

FileTable::~FileTable()
{
  delete[] openFiles;
  delete filePresenceIndicator;
}

/**
 * Add given file to openFiles array.
 *
 * @param file file to add in openFiles list. Could be NULL,
 * in its cases, it is not added and we return false.
 * @return true if file has been added, false otherwise.
 */
bool FileTable::AddFile(OpenFile *file, int sector)
{
  if (file == NULL || filePresenceIndicator->NumSet()==nbFiles)
    return false;

  int index = filePresenceIndicator->Find();

  if (index == -1)
    return false;

  openFiles[index] = new fileOpen();
  openFiles[index]->nbWaiting = 0;
  openFiles[index]->lock = new Lock("File Table Lock");

  openFiles[index]->openFile = file;
  openFiles[index]->numSector = sector;
  openFiles[index]->lock->Acquire();
  openFiles[index]->nbWaiting++;
  return true;
}

/**
 * Remove given file from openFiles array.
 *
 * @param file file to remove from openFiles list. Could be NULL,
 * in its cases, it is not deleted and we return false.
 * @return true if file has been deleted, false otherwise.
 */
bool FileTable::RemoveFile(OpenFile *file)
{
  if (file == NULL)
    return false;

  for (int i = 0; i < nbFiles; i++)
    if (openFiles[i] != NULL && openFiles[i]->openFile == file) {
      openFiles[i]->lock->Release();
      openFiles[i]->nbWaiting--;
      
      if(openFiles[i]->nbWaiting==0){
        delete openFiles[i];
        openFiles[i] = NULL;        
        filePresenceIndicator->Clear(i);
      }
      
      return true;
    }
  return false;
}

/**
 * Search for a correponding sector number in file array and return 
 * openFile resulting. //TODO
 * 
 * @param sector sector number of file header on disk.
 * @return corresponding OpenFile if exists in array, NULL otherwise.
 */
OpenFile* FileTable::getOrCreateOpenFile(int sector) {  
  OpenFile *openfile = NULL;

  for (int i = 0; i < nbFiles; i++)
    if (openFiles[i] != NULL && openFiles[i]->numSector == sector) {
      openfile = openFiles[i]->openFile;
      openFiles[i]->nbWaiting++;
      openFiles[i]->lock->Acquire();
      filePresenceIndicator->Mark(i);
      break;
    }
  if(openfile == NULL){
    openfile = new OpenFile(sector);
    if(!AddFile(openfile,sector))
      return NULL;
  }
  return openfile;
}

/**
 * Search for a correponding sector number in file array and return 
 * openFile resulting.
 * 
 * @param sector sector number of file header on disk.
 * @return corresponding OpenFile if exists in array, NULL otherwise.
 */
OpenFile* FileTable::getFile(int sector) {  
  if (sector == 0 || sector == 1)
    return NULL;
  for (int i = 0; i < nbFiles; i++)
    if (openFiles[i] != NULL && openFiles[i]->numSector == sector)
      return openFiles[i]->openFile;
  return NULL;
}

/**
 * Search for a correponding openFile pointer in file array and return 
 * sector resulting.
 * 
 * @param openFile OpenFile pointer to find in array.
 * @return corresponding sector number of file header on disk, if exists in array,
 * NULL otherwise.
 */
int FileTable::getSector(OpenFile* openFile) {
for (int i = 0; i < nbFiles; i++)
  if (openFiles[i] != NULL && openFiles[i]->openFile == openFile)
    return openFiles[i]->numSector;
return -1;
}