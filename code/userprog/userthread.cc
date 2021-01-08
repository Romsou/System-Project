
// userthread.cc 
// 

#include "thread.h"
#include "system.h"
#include "machine.h"

struct forkArgs
{
  int func;
  int args;
};

/**
 * StartUserThread
 */
static void StartUserThread(int f) {
  //TODO : ...

  machine->Run();
}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f,int arg) {

  struct forkArgs fArgs;
  fArgs.args = arg;
  fArgs.func = f;

  Thread *newThread = new Thread("new_user_thread");
  newThread->Fork(StartUserThread,(int)&fArgs);

  return 0; //Return something about the thread... tid?
}


/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit(){

  delete currentThread->space; //TODO : A vÃ©rifier 
  currentThread->Finish();
}