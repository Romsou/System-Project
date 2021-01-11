#include "thread.h"
#include "system.h"
#include "machine.h"
#include "userthread.h"
#define NBMAXTHREADS 10 //Pour l'instant 10

struct FunctionAndArgs *listOfUserThreads[NBMAXTHREADS] = {};

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
	DEBUG('t',"Call of StartUserThread\n");

	currentThread->space->InitRegisters();
  DEBUG('t',"SUT : registres inities");
  
  int stackaddress = machine->ReadRegister(StackReg) + 16;
  DEBUG('t',"SUT : lecture de la pile");

  struct FunctionAndArgs *st = (struct FunctionAndArgs *)&f;

  DEBUG('t',"SUT : avant l'ecriture");
  machine->WriteRegister(PCReg, st->func);

  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackaddress - 2 * PageSize);
  machine->WriteRegister(4, st->args);

  DEBUG('t',"SUT : avant l'execution");
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
  newThread->Fork(StartUserThread, (int)fArgs);

  if (newThread == NULL)
    return -1;

  for(;;)
    currentThread->Yield();

  

  return thread_id;
}

/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit()
{
  free(listOfUserThreads[currentThread->getTid()]);
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