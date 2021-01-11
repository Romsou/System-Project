#include "thread.h"
#include "system.h"
#include "machine.h"

struct forkArgs
{
  void (*func)(int);
  int fun;
  int args;
  int *pta;
};

void TestZeroPointe(){
  printf("Test zero pointe\n");
}

///Procedure test de StartUserThread
void ThreadTestPrint(int which){
  printf("In ThreadTestPrint\n");
  for(int i = 0 ; i < 6 ; i++){
    printf("Executing thread %d\n",which);
    //PutInt(which);
    //PutChar('\n');
    currentThread->Yield();
  }
}
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

	DEBUG('t',"Call of StartUserThread\n");

  //void *b = (void *)(&args);
  
  struct forkArgs *fa = (struct forkArgs *)(&f);
  //printf("In SUT : %p = %d et %d\n",fa->func,fa->fun,fa->args);
  //printf("Créé %d\n", *((int*)fa->func));
  
  int stackAddress = machine->ReadRegister(StackReg);

  machine->WriteRegister(PCReg, fa->fun);
  machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister(StackReg, stackAddress + 2 * PageSize);

  machine->Run();

}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f, int arg)
{

  struct forkArgs *fArgs = (forkArgs *)malloc(sizeof(forkArgs));
  int a = 2;
  fArgs->pta = &a;
  fArgs->args = arg;
  //fArgs->func = &ThreadTestPrint;
  fArgs->fun = f;
  //printf("In DUTC: %p = %d avec %d\n",fArgs->func,fArgs->fun,fArgs->args);

  Thread *newThread = new Thread("new_user_thread");
	newThread->Fork(StartUserThread, fArgs);

  if (newThread == NULL)
    return -1;

  currentThread->Yield();

  return 0; //Return something about the thread... tid?
}

/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit()
{

  delete currentThread->space; //TODO : A vÃ©rifier
  currentThread->Finish();
}