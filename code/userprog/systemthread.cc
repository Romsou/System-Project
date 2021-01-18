#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"

extern void StartProcess(char *filename);

AddrSpace *getAddressSpace(char *filename){
  OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
    DEBUG ('t',"Unable to open file %s\n", filename);
    return NULL;
      }
    DEBUG('x',"Ouverture fichier %s\n",filename);
    space = new AddrSpace (executable);

    delete executable;
    return space;
}

void launcher(int i){
  DEBUG('x',"Launching of system thread by %s\n",currentThread->getTid());
  currentThread->space->InitRegisters ();  // set the initial register values
  currentThread->space->RestoreState (); // load page table register

  machine->Run ();    // jump to the user progam
}

/**
 * 
 */
int do_SystemThreadCreate(char *s){
  Thread *t = new Thread(s);
  DEBUG('x',"Creating of system thread by %d\n",t->getTid());

  t->space = getAddressSpace(s);
  if(t->space ==NULL)
    return -1;
  //t->Fork(launcher, 0);
  IntStatus oldLevel = interrupt->SetLevel(IntOff);
  scheduler->ReadyToRun(t); // ReadyToRun assumes that interrupts
  // are disabled!

  (void)interrupt->SetLevel(oldLevel); 

  return 0;
}