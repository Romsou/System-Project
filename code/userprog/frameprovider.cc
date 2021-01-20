// frameprovider.cc
//

#include "frameprovider.h"
#include "system.h"
#include "machine.h"

/**
 * Create a frame provider. 
 * 
 * Internally uses bitmap to handle page allocation.
 */
FrameProvider::FrameProvider()
{
  frameTable = new BitMap(NumPhysPages); //nbFrame = nb physique page
}

/**
 * Properly De-allocate a frameProvider.
 */
FrameProvider::~FrameProvider()
{
  delete frameTable;
}

/**
 * Provides an empty frame
 * 
 * Find a free frame and return his index.
 * 
 * @return free physical page number.
 */
int FrameProvider::GetEmptyFrame()
{

  int frameIndex = frameTable->Find();
  if (frameIndex == -1)
    return -1;
  bzero(machine->mainMemory + frameIndex * PageSize, PageSize);
  return frameIndex;
}

/**
 * Free a given frame.
 * 
 * @param frameIndex The index of the frame to free.
 */
void FrameProvider::ReleaseFrame(int frameIndex)
{
  ASSERT(frameIndex >= 0 && frameIndex < NumPhysPages);
  frameTable->Clear(frameIndex);
}

/**
 * Count and return number of free frames.
 * 
 * @return number of free frames. 
 */
int FrameProvider::NumAvailFrame()
{
  return frameTable->NumClear();
}
