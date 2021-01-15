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

#define UserStackSize	3072	//2 * NB_MAX_THREADS * PageSize + 16	// increase this as necessary!
#define NB_MAX_THREADS 10

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

    //Semaphore for synch Halt/Exit
    Semaphore *sem;

    /**
     * Indicates whether the list of user threads is empty
     * 
     * @return A boolean indicating whether the list of user thread is empty
     */
    bool isEmptyUserThread();

    /**
     * Properly removes the current thread from userThread.
     */
    void DeleteThreadFromList(int index);

    /**
     * Put the current thread at in table of userThreads if find a free space.
     * @param index, index of a free space in table
     * @return index of free space found in user thread table, -1 table in full. 
     */
    int AddThreadInList();

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space

    //This array will be used to identify and track the different threads
    struct Thread *userThread[NB_MAX_THREADS] = {};
};

#endif // ADDRSPACE_H
