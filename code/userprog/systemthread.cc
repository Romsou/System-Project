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
  AddrSpace *space = new AddrSpace(executable);
  
  currentThread->space = space;
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  machine->Run();
}

int do_SystemThreadCreate(char *filename)
{
  Thread *process = new Thread(filename);
  process->Fork(startNewProcess, (int) filename);
  return process != NULL;
}
