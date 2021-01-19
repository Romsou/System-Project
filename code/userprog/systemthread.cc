#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"


#define ONOFF 0

extern void StartProcess(char *filename);

typedef struct {
  Thread *t;
  char *filename;
}st_exec;

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
  Thread *t = (Thread *)&i;
  DEBUG('x',"Launching of system thread by %s for %s\n",currentThread->getTid(),t->getName());
  currentThread->SaveUserState();

  t->space->InitRegisters();
  t->SaveUserState();

  IntStatus oldLevel = interrupt->SetLevel(IntOff);
  scheduler->ReadyToRun(t); 
  (void)interrupt->SetLevel(oldLevel); 
    
    //scheduler->Print();
    //printf("\n"); 

  currentThread->RestoreUserState();
  currentThread->space->RestoreState();

  machine->Run ();    // jump to the user progam
}

void starter(int i){
  st_exec *st = (st_exec *)i;

  OpenFile *executable = fileSystem->Open (st->filename);
  AddrSpace *space;

  if (executable == NULL)
    {
    DEBUG ('t',"Unable to open file %s\n", st->filename);
    return;
    }
  DEBUG('x',"Ouverture fichier %s\n",st->filename);
  space = new AddrSpace (executable);
  st->t->space = space;

  delete executable;

  DEBUG('x',"Launching of system thread by %s for %s\n",currentThread->getTid(),st->t->getName());
  currentThread->SaveUserState();

  st->t->space->InitRegisters();
  st->t->SaveUserState();

  IntStatus oldLevel = interrupt->SetLevel(IntOff);
  scheduler->ReadyToRun(st->t); 
  (void)interrupt->SetLevel(oldLevel); 
    
  currentThread->RestoreUserState();
  currentThread->space->RestoreState();
  return;
}

void startNewProcess(int argAddr)
{
  char* filename = (char*) argAddr;
  OpenFile *executable = fileSystem->Open(filename);
  AddrSpace *space = new AddrSpace(executable);
  
  currentThread->space = space;
  DEBUG('x',"Starting of system thread by %s \n",currentThread->getTid());
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  machine->Run();
}

/**
 * 
 */
int do_SystemThreadCreate(char *s){

  if(ONOFF == 0){
    Thread *process = new Thread(s);
    DEBUG('x',"Creating of system thread \"%s\" by \"%s\"\n",
      process->getName(),currentThread->getName());
    process->Fork(startNewProcess, (int) s);
    return process != NULL;

  }else{
    Thread *t = new Thread(s);   
    if(t->getTid() == -1)
      return -1;

    DEBUG('x',"Creating of system thread \"%s\" by \"%s\"\n",t->getName(),currentThread->getName());

    st_exec *st = (st_exec *)malloc(sizeof(st_exec));
    st->t = t;
    st->filename = s;
    t->Fork(starter,(int)st);

    return 0;
  }
}