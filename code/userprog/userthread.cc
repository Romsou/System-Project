#include "thread.h"
#include "system.h"
#include "machine.h"
#include "userthread.h"

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
  DEBUG('t', "Call of StartUserThread\n");

  currentThread->space->InitRegisters();

  int stackaddress = machine->ReadRegister(StackReg);

  machine->WriteRegister(PCReg, currentThread->getFunction());

  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackaddress - 2 * currentThread->getIndex() * PageSize);
  machine->WriteRegister(4, currentThread->getArgs());

  //This will allow us to call UserThreadExit (see exception.cc)
  machine->WriteRegister(RetAddrReg, currentThread->getReturnAddr());

  machine->Run();
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
  int threadIndex = currentThread->space->GetFreeSpotInUserThreadArray();

  if (threadIndex == -1)
  {
    DEBUG('a', "Cannot create more user threads (currentThread->space->listOfUserThreads full)");
    return -1;
  }

  Thread *newThread = new Thread("new_user_thread");
  if (newThread == NULL)
    return -1;

  currentThread->space->putThreadAtIndex(newThread, threadIndex);
  newThread->setTid(newThread->generateTid());  //generate unique tid
  newThread->setPpid(currentThread->getPid());  //retrieve parent process pid
  newThread->setIndex(threadIndex); //met a jour index

  newThread->setFunction(f);
  newThread->setArgs(arg);
  newThread->setReturnAddr(machine->ReadRegister(6)); // Set potentiellement à PCReg

  newThread->Fork(StartUserThread, 0);

  DEBUG('x', "Number of the next free thread_id: %d\n", threadIndex);

  return newThread->getTid();
}

/**
 * do_UserThreadExit erases and properly ends the current thread
 */
void do_UserThreadExit()
{
  for (int i = 0; i < currentThread->getNumberOfWaitingThreads(); i++)
    currentThread->clearWaitingThreads();

  ASSERT(currentThread->getNumberOfWaitingThreads() == 0);
  currentThread->Finish();
}

/**
 * Allows a user thread to wait for the termination of another user thread.
 * @param tid: The tid of the thread
 * @return: 0 on success
 */
int do_UserThreadJoin(int tid)
{
  Thread *t = currentThread->space->getThreadAtId(tid);

  if (t != NULL)
    t->waitThread();

  return 0;
}
