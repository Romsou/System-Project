// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"
#include "synch.h"
#include "bitmap.h"
#include "noff.h"
#include "frameprovider.h"

#define NB_MAX_THREADS 12
#define UserStackSize 2 * (NB_MAX_THREADS+1) * PageSize	// increase this as necessary!

class AddrSpace
{
public:
  AddrSpace(OpenFile *executable); // Create an address space,
  // initializing it with the program
  // stored in the file "executable"
  ~AddrSpace(); // De-allocate an address space

  void InitRegisters(); // Initialize user-level CPU registers,
  // before jumping to user code

  void SaveState();    // Save/restore address space-specific
  void RestoreState(); // info on a context switch

  //Lock for synch Halt/Exit
  Lock *HaltAndExitLock;

  /**
   * Indicates whether the list of user threads is empty
   * 
   * @return A boolean indicating whether the list of user thread is empty
   */
  bool isEmptyUserThread();

  /**
   * Properly removes the current thread from userThreads.
   * 
   * @param index: The index of the thread we want to delete in
   * 				 the array of userThreads.
   */
  void DeleteThreadFromArray(int index);

  /**
   * Put the current thread at in table of userThreads if find a free space.
   */
  int GetFreeSpotInUserThreadArray();

  /**
   * Search in list of threads, those who is corresponding to the id
   * if none thread corresponding, return null
   */
  struct Thread *getThreadAtId(int id);

  /**
   * Put the given thread at index in the array of user threads
   * 
   * @param thread: The thread we want to place in our array.
   * @param index: The index at which we want to put this thread.
   */
  void putThreadAtIndex(Thread *thread, int index);

  bool isValid();

private:
  TranslationEntry *pageTable; // Assume linear page table translation
  unsigned int numPages;       // Number of pages in the virtual
                               // for now!
                               // address space

  //This array will be used to identify and track the different threads
  Thread **userThreads;
  int nbUserThreads;

  unsigned int estimateAddressSpaceSize(NoffHeader noffh);
  unsigned int roundUpAdressSpaceSize(unsigned int size);

  void allocatePages();
  void initializePage(unsigned int index);

  // Add pageTable and numPages as parameters and make static if there's a problem
  void copyFromExecToMemory(OpenFile *executable, Segment segment);

  void createUserThreads();
  void initializeUserThreads();
};

#endif // ADDRSPACE_H
