// userthread.cc

#include "thread.h"
#include "system.h"
#include "machine.h"

#define NBMAXTHREADS 10 //Pour l'instant 10

struct forkArgs
{
  int func;
  int args;
};

struct forkArgs *listOfUserThreads[NBMAXTHREADS] = {};

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
  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackAddress + 2 * PageSize);

  machine->Run();
}

int findFreeThread()
{
  int i = 0;
  while ((i < NBMAXTHREADS) && listOfUserThreads[i] != NULL)
  {
    i++;
  }
  if (i < NBMAXTHREADS)
  {
    return i;
  }
  DEBUG('a', "Cannot create more user threads (listOfUserThreads full)");
  return -1;
}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f, int arg)
{

  struct forkArgs *fArgs = (forkArgs *)malloc(sizeof(forkArgs));
  fArgs->args = arg;
  fArgs->func = f;

  int thread_id = findFreeThread();
  if (thread_id == -1)
  {
    return -1;
  }
  listOfUserThreads[thread_id] = fArgs;

  Thread *newThread = new Thread("new_user_thread" + thread_id);
  newThread->setTid(thread_id);
  newThread->Fork(StartUserThread, (int)fArgs);

  if (newThread == NULL)
    return -1;

  currentThread->Yield();

  return thread_id;
}

/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit()
{

  delete listOfUserThreads[currentThread->getTid()];
  listOfUserThreads[currentThread->getTid()] = NULL;
  //delete currentThread->space; //TODO : A vÃ©rifier
  currentThread->Finish();
}

int do_UserThreadJoin(int tid)
{

  while (listOfUserThreads[tid] != NULL)
  {
    currentThread->Yield();
  }

  return 0;

}