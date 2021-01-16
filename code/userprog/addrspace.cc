// addrspace.cc
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

#include <strings.h> /* for bzero */

/**
 * ReadAtVirtual
 * @param executable
 * @param virtualaddr
 * @param numBytes
 * @param position
 * @param pageTable
 * @param numPages
 * 
 */
/*
static void ReadAtVirtual(OpenFile *executable, int virtualaddr,
													int numBytes, int position, TranslationEntry *pageTable, unsigned numPages)
{
	char buffer[numBytes];
	executable->ReadAt(buffer, numBytes, position);

	int i;
	for (i = 0; i < numBytes && machine->WriteMem(virtualaddr + i, 1, buffer[i]); i++)
		;
}*/

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader(NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr =
		WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable)
{
	NoffHeader noffH;
	unsigned int i, size;

	executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
	if ((noffH.noffMagic != NOFFMAGIC) &&
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
		SwapHeader(&noffH);
	ASSERT(noffH.noffMagic == NOFFMAGIC);

	// how big is address space?
	size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize; // we need to increase the size
	// to leave room for the stack
	numPages = divRoundUp(size, PageSize);
	size = numPages * PageSize;

	ASSERT(numPages <= NumPhysPages); // check we're not trying
	// to run anything too big --
	// at least until we have
	// virtual memory

	DEBUG('a', "Initializing address space, num pages %d, size %d\n",
		  numPages, size);
	// first, set up the translation
	pageTable = new TranslationEntry[numPages];

	for (i = 0; i < numPages; i++)
	{
		pageTable[i].virtualPage = i; // for now, virtual page # = phys page #
		pageTable[i].physicalPage = i;
		pageTable[i].valid = TRUE;
		pageTable[i].use = FALSE;
		pageTable[i].dirty = FALSE;
		pageTable[i].readOnly = FALSE; // if the code segment was entirely on
									   // a separate page, we could set its
									   // pages to be read-only
	}

	// zero out the entire address space, to zero the unitialized data segment
	// and the stack segment
	bzero(machine->mainMemory, size);

	// then, copy in the code and data segments into memory
	if (noffH.code.size > 0)
	{
		DEBUG('a', "Initializing code segment, at 0x%x, size %d\n",
			  noffH.code.virtualAddr, noffH.code.size);
		// ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, noffH.code.inFileAddr, pageTable, 0); //les zero sont a changer mais par quoi?

		executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
						   noffH.code.size, noffH.code.inFileAddr);
	}
	if (noffH.initData.size > 0)
	{
		DEBUG('a', "Initializing data segment, at 0x%x, size %d\n",
			  noffH.initData.virtualAddr, noffH.initData.size);

		//ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, noffH.initData.inFileAddr, pageTable, 1); //les zero sont a changer mais par quoi?

		executable->ReadAt(&(machine->mainMemory
								 [noffH.initData.virtualAddr]),
						   noffH.initData.size, noffH.initData.inFileAddr);
	}
	sem = new Semaphore("sem", 0);

	userThreads = new Thread *[NB_MAX_THREADS];
	for (int k = 0; k < NB_MAX_THREADS; k++)
		userThreads[i] = NULL;
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
	// LB: Missing [] for delete
	// delete pageTable;
	delete[] pageTable;
	// End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void AddrSpace::InitRegisters()
{
	int i;

	for (i = 0; i < NumTotalRegs; i++)
		machine->WriteRegister(i, 0);

	// Initial program counter -- must be location of "Start"
	machine->WriteRegister(PCReg, 0);

	// Need to also tell MIPS where next instruction is, because
	// of branch delay possibility
	machine->WriteRegister(NextPCReg, 4);

	// Set the stack register to the end of the address space, where we
	// allocated the stack; but subtract off a bit, to make sure we don't
	// accidentally reference off the end!
	machine->WriteRegister(StackReg, numPages * PageSize - 16);
	DEBUG('a', "Initializing stack register to %d\n",
		  numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
	machine->pageTable = pageTable;
	machine->pageTableSize = numPages;
}

/**
 * Checks whether the array of user thread is empty.
 * 
 * This works by checking for each box in our array
 * whether it is NULL. If we find a userThreads, we exit
 * early.
 * 
 * @return a bool, true if userThreads is empty, false otherwise.
 */
bool AddrSpace::isEmptyUserThread()
{
	ASSERT(userThreads != NULL);
	for (int i = 0; i < NB_MAX_THREADS; i++)
		if (userThreads[i] != NULL)
			return false;

	return true;
}

/**
 * Removes the userThread at the given index in userThreads.
 * 
 * @param index: The index of the thread we want to delete in
 * 				 the array of userThreads.
 */
void AddrSpace::DeleteThreadFromArray(int index)
{
	userThreads[index] = NULL;
}

/**
 * Find the first free spot in userThreads.
 * 
 * @param index, index of a free space in table
 * @return index of free space found in user thread table, -1 table in full. 
 */
int AddrSpace::GetFreeSpotInUserThreadArray()
{
	for (int i = 0; i < NB_MAX_THREADS; i++)
		if (userThreads[i] == NULL)
			return i;

	DEBUG('a', "Cannot create more user threads (userThreads full)");
	return -1;
}

/**
 * Search for a thread with the specified id in the array of user threads
 * 
 * We iterate over all of the array and checks whether the thread at this
 * spot has the given id.
 * 
 * @param id The unique id of the thread we're looking for.
 * @return A pointer to the user thread correspondin to id.
 */
Thread *AddrSpace::getThreadAtId(int id)
{
	for (int i = 0; i < NB_MAX_THREADS; i++)
			if (userThreads[i] != NULL && userThreads[i]->getTid() == id)
					return userThreads[i];
	return NULL;
}

/**
 * Put the given thread at index in the array of user threads
 * 
 * @param thread: The thread we want to place in our array.
 * @param index: The index at which we want to put this thread.
 */
void AddrSpace::putThreadAtIndex(Thread* thread, int index)
{
	ASSERT(index >= 0 && index < NB_MAX_THREADS);
	userThreads[index] = thread;
}
