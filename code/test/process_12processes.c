#include "syscall.h"

static void print(void *arg)
{
  PutInt(2);
}

int main()
{
  int i;  
  for (i=0; i<11; i++) {
    ForkExec("process_12threads");
  }

  //11 ForkExec process + 1 main process = 12 process
  for (i=0; i<12; i++) {
     UserThreadCreate(print, 0);
  }
  return 0;
}