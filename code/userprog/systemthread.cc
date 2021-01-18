#include "system.h"
#include "thread.h"

extern void StartProcess (char *filename);

/**
 * 
 */
int do_SystemThreadCreate(char  *s){
  Thread *t = new Thread("ForkThread");
  t->Fork((VoidFunctionPtr)StartProcess, s);
  return 0;
}