#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"
#include "addrspace.h"
#include <string.h>

static void startNewProcess(int argAddr)
{
  char* filename = (char*) argAddr;
  OpenFile *executable = fileSystem->Open(filename);
  ASSERT(executable != NULL);

  AddrSpace *space = new AddrSpace(executable);
  delete executable;		// close file
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
  process->setPid(process->generatePid());
  processTable->add(process);
  process->Fork(startNewProcess, (int)filename);

  return process->getPid();
}

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
  OpenFile *executable = fileSystem->Open(filename);
  ASSERT(executable != NULL);

  AddrSpace *space = new AddrSpace(executable);
#ifdef FILSYS
    fileSystem->Close(executable);
#else 
    delete executable;
#endif //FILSYS

  if(!space->isValid()){
    space = NULL;
  }else{
    space = space;
  }
  free(filename);
  
  if(currentThread->space == NULL)
    return -1;

  Thread *process = new Thread(filename);
  //startNewSpace(process,filename);
  
  process->setPid(process->generatePid());
  processTable->add(process);
  
  process->Fork(startNewProcess, (int)filename);
  process->space = space;
  if(process->space==NULL)
    return -1;

  //(void)interrupt->SetLevel(oldLevel);
  return process->getPid();
}
*/