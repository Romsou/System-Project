// frameprovider.cc
//

#include "frameprovider.h"
#include "system.h"

/**
 * Create a frame provider. 
 * 
 * Internally uses bitmap to handle page allocation.
 */
FrameProvider::FrameProvider()
{
  bm = new BitMap(NumPhysPages); //nbFrame = nb physique page
}

/**
 * Properly De-allocate a frameProvider.
 */
FrameProvider::~FrameProvider()
{
  delete bm;
}

/**
 * Provide an empty frame
 * 
 * Find a free frame and return his index.
 * 
 * @return free physical page number.
 */
int FrameProvider::GetEmptyFrame()
{
  int frameIndex = bm->Find();
  bzero(machine->mainMemory + PageSize * frameIndex, PageSize);
  return frameIndex;
}

/**
 * Free a given physical page.
 * 
 * @param frameIndex the physical page's number to free.
 */
bool FrameProvider::ReleaseFrame(int frameIndex)
{
  bm->Clear(frameIndex);
  return true; //Dans quelle ca peut pas libérer? A part si frameIndex est > ou < à nbFrame?
}

/**
 * Count and return number of free page in physical memory.
 * 
 * @return number of free physical page. 
 */
int FrameProvider::NumAvailFrame()
{
  return bm->NumClear();
}
