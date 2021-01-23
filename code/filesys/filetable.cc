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
  if (file == NULL && filePresenceIndicator->NumSet()==nbFiles)
    return false;

  int index = filePresenceIndicator->Find();

  if (index == -1)
    return false;

  openFiles[index]->openFile = file;
  openFiles[index]->numSector = sector;
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
      openFiles[i] = NULL;
      delete file;
      filePresenceIndicator->Clear(i);
      return true;
    }
  return false;
}

/**
 * Search for a correponding sector number in file array and return 
 * openFile resulting.
 * 
 * @param sector sector number of file header on disk.
 * @return corresponding OpenFile if exists in array, NULL otherwise.
 */
OpenFile* FileTable::getFile(int sector) {
  for (int i = 0; i < nbFiles; i++)
    if (openFiles[i] != NULL && openFiles[i]->numSector == sector) {
      return openFiles[i]->openFile;
    }
  return NULL;
}
