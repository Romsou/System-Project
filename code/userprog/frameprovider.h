// frameprovider.h
//

#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "bitmap.h"
#include "machine.h"

class FrameProvider
{
  public:
	FrameProvider();
	~FrameProvider();

	/**
	 * Find and return return a free physical page's number. If no physical page are
	 * free, return -1.
	 * 
	 * @return free physical page number.
	 */
	int GetEmptyFrame();

	/**
	 * Free a given physical page.
	 * 
	 * @param frameIndex the physical page's number to free.
	 */
	void ReleaseFrame(int frameIndex);

	/**
	 * Count and return number of free page in physical memory.
	 * 
	 * @return number of free physical page. 
	 */
	int NumAvailFrame();

private:
	BitMap* physMemBitMap;
};

#endif //FRAMEPROVIDER_H