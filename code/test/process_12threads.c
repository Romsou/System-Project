#include "syscall.h"

static void print(void *arg)
{
  PutInt(1);
}

int main()
{
  int i;
  for (i=0; i<12; i++) {
     UserThreadCreate(print, 0);
  }
  End();
  return 0;
}