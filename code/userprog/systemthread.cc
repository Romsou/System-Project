#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"
#include "addrspace.h"
#include <string.h>

void startNewProcess(int argAddr)
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

  process->Fork(startNewProcess, (int) filename);
  return process != NULL;
}
