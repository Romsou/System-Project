#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "thread.h"
#include "synch.h"
#include "bitmap.h"

class ProcessTable
{
public:
    ProcessTable(int nbMaxProcess);
    ~ProcessTable();
    
    bool add(Thread *process);

    void remove(int id);
    void removeAt(int index);

    int getNumberOfActiveProcesses();

    // void preventsHalt();
    // void allowHalt();

private:
    int nbProcess;
    Thread **processes;
    BitMap* processPresenceIndicator;

    // Semaphore* haltLock;
};

#endif