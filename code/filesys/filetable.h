#ifndef FILETABLE_H
#define FILETABLE_H

#include "bitmap.h"

//We can identify each file, because each file has its own 
//sector number for its file header. 
struct fileOpen
{
  int numSector;
  OpenFile *openFile;
};

class FileTable
{
public:
  FileTable(int nbMaxFiles);
  ~FileTable();

  /**
   * Add given file to openFiles array.
   *
   * @param file file to add in openFiles list. Could be NULL,
   * in its case, it is not added and we return false.
   * @return true if file has been added, false otherwise.
   */
  bool AddFile(OpenFile *file, int sector);

  /**
   * Remove given file from openFiles array.
   *
   * @param file file to remove from openFiles list. Could be NULL,
   * in its case, it is not deleted and we return false.
   * @return true if file has been deleted, false otherwise.
   */
  bool RemoveFile(OpenFile *file);

  /**
   * Search for a correponding sector number in file array and return 
   * openFile resulting.
   * 
   * @param sector sector number of file header on disk.
   * @return corresponding OpenFile if exists in array, NULL otherwise.
   */
  OpenFile* getFile(int sector);

  /**
   * Search for a correponding openFile pointer in file array and return 
   * sector resulting.
   * 
   * @param openFile OpenFile pointer to find in array.
   * @return corresponding sector number of file header on disk, if exists in array,
   * NULL otherwise.
   */
  int getSector(OpenFile* openFile);

private:
  int nbFiles;
  struct fileOpen **openFiles;
  BitMap *filePresenceIndicator;
};

#endif //FILETABLE_H