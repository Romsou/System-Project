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

  machine->WriteRegister(PCReg, ((FunctionAndArgs *)f)->func);

  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackaddress - 2 * (currentThread->getTid()+1)* PageSize);
  machine->WriteRegister(4, ((FunctionAndArgs *)f)->args);

  //This will allow us to call UserThreadExit (see exception.cc)
  machine->WriteRegister(RetAddrReg, ((FunctionAndArgs *)f)->end);

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
  struct FunctionAndArgs *fArgs = (FunctionAndArgs *)malloc(sizeof(FunctionAndArgs));
  fArgs->args = arg;
  fArgs->func = f;
  //We read the 6th register, as it contains the call of UserThreadExit (see start.S)
  fArgs->end = machine->ReadRegister(6);

  int thread_id = currentThread->space->AddThreadInList();
  
  if (thread_id == -1)
    return -1;

  Thread *newThread = new Thread("new_user_thread" + thread_id);
  newThread->setTid(thread_id);
  newThread->Fork(StartUserThread, (int)fArgs);
  DEBUG('x',"Number of the next free thread_id: %d\n" , thread_id);

  if (newThread == NULL)
    return -1;

  return thread_id;
}

/**
 * do_UserThreadExit erases and properly ends the current thread
 */
void do_UserThreadExit()
{
  currentThread->Finish();
}

/**
 * Allows a user thread to wait for the termination of another user thread.
 * 
 * This function shall wait until a thread is finished (noticeable by checking the array
 * listOfUserThreads). do_UserThreadExit is the function that marks the thread as finished by setting
 * to zero the element tid of the array.
 * @param arg: The tid of the thread
 * @return: 0 on success
 */
int do_UserThreadJoin(int tid)
{
  return 0;
}
