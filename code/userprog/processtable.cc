#include "processtable.h"

ProcessTable::ProcessTable(int nbMaxProcess)
{
    nbProcess = nbMaxProcess;   //nbProcess = NB_MAX_PROCESS
    processes = new Thread *[nbProcess];
    for (int i = 0; i < nbProcess; i++)
        processes[i] = NULL;

    processPresenceIndicator = new BitMap(nbProcess);
    // haltLock = new Semaphore("Halt lock", 0);
}

ProcessTable::~ProcessTable()
{
    delete[] processes;
    delete processPresenceIndicator;
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
    if (getNumberOfActiveProcesses() == nbProcess)
       return false;

    int index = processPresenceIndicator->Find();

    if (index == -1)
        return false;


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
    for (int i = 0; i < nbProcess; i++)
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
    ASSERT(index >= 0 && index < nbProcess);
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