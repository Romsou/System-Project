#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "systemthread.h"

void StartProcess (char *filename)
{
    
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

/**
 * 
 */
int do_SystemThreadCreate(char *s){
  Thread *t = new Thread("ForkThread");
  t->Fork((VoidFunctionPtr)StartProcess, s);
  return 0;
}