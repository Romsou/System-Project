// frameprovider.cc
//

#include "frameprovider.h"
#include "system.h"
#include "machine.h"

/**
 * Create a frame provider. Intialize bitMap for handle a given number of 
 * physical pages.
 * 
 * @param nbFrame number of physical page in memory.
 * @return FrameProvider created.
 */
FrameProvider::FrameProvider(int nbFrame) {
  physMemBitMap = new BitMap(nbFrame); //nbFrame = nb physical pages
  bzero(machine->mainMemory, PageSize*NumPhysPages);
}

/**
 * Properly De-allocate a frameProvider.
 */
FrameProvider::~FrameProvider() {
  delete physMemBitMap;
}


/**
 * Find and return return a free physical page's number. If no physical page are
 * free, return -1.
 * 
 * @return free physical page number.
 */
int FrameProvider::GetEmptyFrame(){
  int frameIndex = physMemBitMap->Find();
  if (frameIndex == -1)
    return frameIndex;

  bzero(machine->mainMemory + PageSize*frameIndex, PageSize);
  return frameIndex;
}

/**
 * Free a given physical page.
 * 
 * @param frameIndex the physical page's number to free.
 */
void FrameProvider::ReleaseFrame(int frameIndex){
  ASSERT(frameIndex>0);
  ASSERT(frameIndex<physMemBitMap->NumBits());

  physMemBitMap->Clear(frameIndex);
}

/**
 * Count and return number of free page in physical memory.
 * 
 * @return number of free physical page. 
 */
int FrameProvider::NumAvailFrame(){
  return physMemBitMap->NumClear();
}
