#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"


#define ONOFF 1

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

  if(ONOFF == 0){
    Thread *t = new Thread(s);
    if (t->getTid() == -1) 
      return -1;
    
    DEBUG('t',"Creating of system thread by %d\n",currentThread->getTid());
    t->Fork((VoidFunctionPtr)StartProcess, (int)s);

    return t->getTid();

  }else{
    Thread *t = new Thread(s);
    DEBUG('x',"Creating of system thread by %s\n",t->getName());

    t->space = getAddressSpace(s);
    if(t->space ==NULL)
      return -1;
    
    currentThread->SaveUserState();

    t->space->InitRegisters();
    t->SaveUserState();

    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    scheduler->ReadyToRun(t); 
    (void)interrupt->SetLevel(oldLevel); 
    
    scheduler->Print();
    printf("\n"); 

    currentThread->RestoreUserState();
    currentThread->space->RestoreState();

    return 0;
  }
}