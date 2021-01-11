// userthread.cc

#include "thread.h"
#include "system.h"
#include "machine.h"

struct forkArgs
{
  int func;
  int args;
};

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
static void StartUserThread(int f)
{
  int stackAddress = machine->ReadRegister(StackReg);

  machine->WriteRegister(PCReg, ((forkArgs *)f)->func);
  machine->WriteRegister(4, ((forkArgs *)f)->args);
  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackAddress + 2 * PageSize);

  machine->Run();
}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f, int arg)
{

  struct forkArgs *fArgs = new forkArgs();
  fArgs->args = arg;
  fArgs->func = f;

  Thread *newThread = new Thread("new_user_thread");
  newThread->Fork(StartUserThread, (int)fArgs);

  if (newThread == NULL)
    return -1;

  // currentThread->Yield();

  return 0; //Return something about the thread... tid?
}

/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit()
{
  currentThread->Finish();
  delete currentThread->space;
}