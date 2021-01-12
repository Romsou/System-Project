// userthread.cc

#include "thread.h"
#include "system.h"
#include "machine.h"
#include "userthread.h"

struct FunctionAndArgs *listOfUserThreads[NB_MAX_THREADS] = {};

bool isEmptyListOfUserThreads()
{
  if (listOfUserThreads == NULL)
    return true;

  int i = 0;
  while ((i < NB_MAX_THREADS)) {
    if (listOfUserThreads[i] != NULL)
      return false;
  
    i++;
  }

  return true;
}

/**
 * Starts a new user thread with function f
 * 
 * This function works by initializing the machine environment
 * for the f function to work properly. It then proceeds to 
 * initialize the PC register with the address of f so that we can
 * jump to the user code once it is called.
 * 
 * @param f: The address of a structure containing the function and the args
 *           we want to jump to.
 */
static void StartUserThread(int f)
{
  // Used to put the StackReg at the end of the address space
  currentThread->space->InitRegisters();
  
  int stackaddress = machine->ReadRegister(StackReg) + 16;
  
  machine->WriteRegister(PCReg, ((FunctionAndArgs *)f)->func);
  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackaddress - 2 * PageSize);
  machine->WriteRegister(4, ((FunctionAndArgs *)f)->args);

  machine->Run();
}

int findFreeThread()
{
  int i = 0;
  while ((i < NB_MAX_THREADS) && listOfUserThreads[i] != NULL)
  {
    i++;
  }
  if (i < NB_MAX_THREADS)
  {
    return i;
  }
  DEBUG('a', "Cannot create more user threads (listOfUserThreads full)");
  return -1;
}

/**
 * Create a new user thread and puts it in the ready list.
 * 
 * @param f: The function we want to create a user thread for.
 * @param arg: The argument we want to pass to f
 * @return: 0 or -1 if the creation of the thread fails 
 */
int do_UserThreadCreate(int f, int arg)
{

  struct FunctionAndArgs *fArgs = (FunctionAndArgs *)malloc(sizeof(FunctionAndArgs));
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

  //for(;;)
   // currentThread->Yield();

  return thread_id;
}

void DeleteThreadFromList() {
  delete listOfUserThreads[currentThread->getTid()];
  listOfUserThreads[currentThread->getTid()] = NULL;
}

/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit()
{
  currentThread->Finish();
  delete currentThread->space; //TODO : A vÃ©rifier
}

int do_UserThreadJoin(int tid)
{
  while (listOfUserThreads[tid] != NULL)
  {
    currentThread->Yield();
  }

  return 0;
}

extern void do_WakeUp() {
  //....
}
