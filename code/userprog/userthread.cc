#include "copyright.h"
#include "utility.h"

int 
do_UserThreadCreate(int f,int arg)
{
	//preparer environment 
  Thread *newThread = new Thread("new_user_thread");
  newThread->space = currentThread->space;
  newThread->Fork(StartUserThread,f);
  return arg;
}