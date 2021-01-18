#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "systemthread.h"


int do_SystemThreadCreate(char  *s){
	OpenFile *executable = fileSystem->Open (s);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", s);
	  return -1;
      }

    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		

    space->InitRegisters ();	
    space->RestoreState ();	



    machine->Run ();
    return 0; //TODO
}