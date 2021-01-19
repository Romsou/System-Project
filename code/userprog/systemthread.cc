#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"

extern void StartProcess(char *filename);

/**
 * 
 */
int do_SystemThreadCreate(char *s){
  Thread *t = new Thread(s);
  if (t->getTid() == -1) 
    return -1;
  
  DEBUG('t',"Creating of system thread by %d\n",currentThread->getTid());
  t->Fork((VoidFunctionPtr)StartProcess, (int)s);

  return t->getTid();
}