#include "copyright.h"
#include "system.h"
#include "thread.h"
#include "systemthread.h"

extern void StartProcess(char *filename);

void ext_StartProcess(int i){
  DEBUG('x',"fonction test %d\n",i);

}


/**
 * 
 */
int do_SystemThreadCreate(char *s){
  //Thread *t = new Thread("ForkThread");
  DEBUG('t',"Creating of system thread by %d\n",currentThread->getTid());
  currentThread->Fork(ext_StartProcess, 5);

  return 0;
}