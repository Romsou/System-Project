#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"
#include "addrspace.h"
#include <string.h>

/*
static void startNewProcess(int argAddr)
{
  char* filename = (char*) argAddr;
  OpenFile *executable = fileSystem->Open(filename);
  ASSERT(executable != NULL);

  AddrSpace *space = new AddrSpace(executable);
#ifdef FILSYS
    fileSystem->Close(executable);
#else 
    delete executable;
#endif //FILSYS
  ASSERT(space != NULL);

  currentThread->space = space;
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  free(filename);

  machine->Run();
  ASSERT (FALSE);		// machine->Run never returns;
}
int do_SystemThreadCreate(char *filename)
{
  Thread *process = new Thread(filename);
  if (process == NULL)
    return -1;
  
  process->setPid(process->generatePid());
  processTable->add(process);
  process->Fork(startNewProcess, (int)filename);

  return process->getPid();
}*/



static void startNewProcess(int argAddr)
{
  AddrSpace *space = (AddrSpace*) argAddr;
  currentThread->space = space;
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();
  machine->Run();
  ASSERT (FALSE);		// machine->Run never returns; 
}

int do_SystemThreadCreate(char *filename)
{
  //Try to create addrSpace
  IntStatus oldLevel = interrupt->SetLevel(IntOff);

  OpenFile *executable = fileSystem->Open(filename);
  if(executable==NULL)
    return -1;

  AddrSpace *space = new AddrSpace(executable);

#ifdef FILSYS
  fileSystem->Close(executable);
#else 
  delete executable;
#endif //FILSYS
  
  if(!space->isValid())
    return -1;

  Thread *process = new Thread(filename);
  
  process->setPid(process->generatePid());
  processTable->add(process);
  process->Fork(startNewProcess, (int)space);
  //process->space->InitRegisters();
  //process->space->RestoreState();

  free(filename);

  (void)interrupt->SetLevel(oldLevel);
  return process->getPid();
}
