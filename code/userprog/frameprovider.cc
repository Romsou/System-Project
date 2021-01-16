// frameprovider.cc
//

#include "frameprovider.h"
#include "system.h"

/**
 * Create a frame provider. Intialize bitMap for handle a given number of 
 * physical pages.
 * 
 * @param nbFrame number of physical page in memory.
 * @return FrameProvider created.
 */
FrameProvider::FrameProvider(int nbFrame) {
  bm = new BitMap(nbFrame); //nbFrame = nb physique page
  bzero(machine->mainMemory, PageSize*NumPhysPages);
}

/**
 * Properly De-allocate a frameProvider.
 */
FrameProvider::~FrameProvider() {
  delete bm;
}


/**
 * Find and return return a free physical page's number. If no physical page are
 * free, return -1.
 * 
 * @return free physical page number.
 */
int FrameProvider::GetEmptyFrame(){
  int frameIndex = bm->Find();
  bzero(machine->mainMemory + PageSize*frameIndex, PageSize);
  return frameIndex;
}

/**
 * Free a given physical page.
 * 
 * @param frameIndex the physical page's number to free.
 */
bool FrameProvider::ReleaseFrame(int frameIndex){
  bm->Clear(frameIndex);
  return true;  //Dans quelle ca peut pas libérer? A part si frameIndex est > ou < à nbFrame?
}

/**
 * Count and return number of free page in physical memory.
 * 
 * @return number of free physical page. 
 */
int FrameProvider::NumAvailFrame(){
  return bm->NumClear();
}
