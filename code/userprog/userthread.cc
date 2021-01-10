// userthread.cc 
 
#include "thread.h"
#include "system.h"
#include "machine.h"

struct forkArgs
{
  int func;
  int args;
};

struct forkArgs* fArgs;

/**
 * Starts a new user thread with function f
 * 
 * This function works by initializing the machine environment
 * for the f function to work properly. It then proceeds to 
 * initialize the PC register with the address of f so that we can
 * jump to the user code once it is called.
 * 
 * @param f: The address of the function we want to jump to
 */
static void StartUserThread(int f) {

  currentThread->space->SaveState();
  currentThread->space->InitRegisters();
  //currentThread->space->RestoreState();

  //machine->ReadRegister(StackReg);
  machine->WriteRegister(PCReg, fArgs->func);
  machine->Run();
}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f,int arg) {

  fArgs = new forkArgs;
  fArgs->args = arg;
  fArgs->func = f;

  Thread *newThread = new Thread("new_user_thread");
  newThread->Fork(StartUserThread,(int)fArgs);
  currentThread->Yield();
  return 0; //Return something about the thread... tid?
}


/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit(){

  delete currentThread->space; //TODO : A vÃ©rifier 
  currentThread->Finish();
}