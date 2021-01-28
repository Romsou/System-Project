#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"
#include "addrspace.h"
#include <string.h>

/*
static void startNewProcess(int argAddr)
{
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();
  machine->Run();
  ASSERT (FALSE);		// machine->Run never returns;
}
int do_SystemThreadCreate(char *filename)
{
  //Try to create AddrSpace
  OpenFile *executable = fileSystem->Open(filename);
  ASSERT(executable != NULL);

  AddrSpace *space = new AddrSpace(executable);
  delete executable;		// close file
  free(filename);
  ASSERT(space != NULL);

  if(!space->isValid()){
    space = NULL;
    // currentThread->Finish()
    interrupt->Halt();
  }else{
    space = space;
  }

  Thread *process = new Thread(filename);
  process->setPid(process->generatePid());
  processTable->add(process);
  process->Fork(startNewProcess, (int)filename);
  process->space = space;

  return process->getPid();
}*/

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

  if(!space->isValid()){
    interrupt->Halt();
  }else{
    space = space;
  }
  free(filename);
  
  currentThread->space = space;
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();
  
  machine->Run();
  ASSERT (FALSE);		// machine->Run never returns; 
}

int do_SystemThreadCreate(char *filename)
{
  if (currentThread->getPid() == -1)
    return -1;
  
  Thread *process = new Thread(filename);
  //startNewSpace(process,filename);
  
  process->setPid(process->generatePid());
  if (!processTable->add(process))
    return -1;

  process->Fork(startNewProcess, (int)filename);

  //(void)interrupt->SetLevel(oldLevel);
  return process->getPid();
}
