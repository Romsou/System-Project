#include "processtable.h"

ProcessTable::ProcessTable()
{
    processes = new Thread *[NB_MAX_PROCESS];
    for (int i = 0; i < NB_MAX_PROCESS; i++)
        processes[i] = NULL;

    processPresenceIndicator = new BitMap(NB_MAX_PROCESS);
    // haltLock = new Semaphore("Halt lock", 0);
}

ProcessTable::~ProcessTable()
{
    delete processes;
    // delete haltLock;
}

/**
 * Adds a process to the process table
 * 
 * @param process: The thread we want to add in our process table
 * @return True if the addition was successful, false otherwise.
 */
bool ProcessTable::add(Thread *process)
{
    int index = processPresenceIndicator->Find();

    if (index == -1)
        return false;

    return addAt(process, index);
}

/**
 * Adds a process to the given index.
 * 
 * Performs some security checks along the way.
 * Checks if the table is not yet full, if the process
 * is indeed a process and not a user thread, and
 * checks if a process is already there or not. If it is
 * we do not overwrite it.
 * 
 * @param process: The process to add to the table.
 * @param index: The index at which to put process.
 */
bool ProcessTable::addAt(Thread *process, int index)
{
    ASSERT(index >= 0 && index < NB_MAX_PROCESS);

    //TODO : mb remove this function and put all following if in add, before     int index = processPresenceIndicator->Find();??
    //if (getNumberOfActiveProcesses() == NB_MAX_PROCESS) //Same as below, Find already mark a bit
    //    return false;
    //if (process->getPpid() == -1)     //For now, no PPid
    //    return false;

    //if (processPresenceIndicator->Test(index)) //Doesn't work if we came from add
    //    return false; // because find already Mark a bit then this test return true

    processes[index] = process;
    processPresenceIndicator->Mark(index);

    return true;
}

/**
 * Remove the process with the given id.
 *
 * @param id: The PID of the process to remove.
 */
void ProcessTable::remove(int id)
{
    for (int i = 0; i < NB_MAX_PROCESS; i++)
        if (processes[i] != NULL && processes[i]->getPid() == id)
            removeAt(i);
}

/**
 * Remove the process at the given index in the process table
 * 
 * @param index: The index at which to remove the process.
 */
void ProcessTable::removeAt(int index)
{
    ASSERT(index >= 0 && index < NB_MAX_PROCESS);
    processes[index] = NULL;
    processPresenceIndicator->Clear(index);
}

/**
 * Returns the number of processes in the table.
 * 
 * @return The number of processes in the process table.
 */
int ProcessTable::getNumberOfActiveProcesses()
{
    return processPresenceIndicator->NumSet();
}

/**
 * Prevents the program to shutdown the machine
 * as long as at least one process is still running.
 */
/*void ProcessTable::preventsHalt()
{
    if (getNumberOfActiveProcesses() > 0)
        haltLock->P();
}*/

/**
 * Allows the program to shut down the machine if there
 * are no processes left to run.
 */
/*void ProcessTable::allowHalt()
{
    if (getNumberOfActiveProcesses() == 0)
        haltLock->V();
}*/