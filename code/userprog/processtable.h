#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "thread.h"
#include "synch.h"
#include "bitmap.h"

#define NB_MAX_PROCESS 3  //TODO:MB move it into system.h?

class ProcessTable
{
public:
    ProcessTable();
    ~ProcessTable();
    
    bool add(Thread *process);
    bool addAt(Thread *process, int index);

    void remove(int id);
    void removeAt(int index);

    int getNumberOfActiveProcesses();

    // void preventsHalt();
    // void allowHalt();

private:
    Thread **processes;
    BitMap* processPresenceIndicator;

    // Semaphore* haltLock; //TODO remove semaphore?
};

#endif