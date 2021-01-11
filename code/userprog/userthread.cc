// userthread.cc 
#include "syscall.h" 
#include "thread.h"
#include "system.h"
#include "machine.h"

struct forkArgs
{
  void (*func)(int);
  int args;
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
 * StartUserThread
 */
static void StartUserThread(int args) {
  DEBUG('t',"Call of StartUserThread\n");

  //void *b = (void *)(&args);
  
  struct forkArgs *fa = (struct forkArgs *)(void *)(&args);
  printf("In SUT : %p et %d\n",fa->func,fa->args);
  
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  (fa->func)(fa->args);
  //machine->ReadRegister(StackReg);
  //machine->WriteRegister(PCReg, f);
  //machine->Run();
}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f,int arg) {
  DEBUG('t',"Call of do_UserThreadCreate\n");
  struct forkArgs *fArgs = (struct forkArgs *)malloc(sizeof(struct forkArgs));
  fArgs->args = 5;
  fArgs->func = &ThreadTestPrint;
  printf("Une fonction %p ave comme arg %d\n",fArgs->func,fArgs->args);

  Thread *newThread = new Thread("new_user_thread");
  newThread->Fork(StartUserThread,fArgs);
  //StartUserThread(fArgs.func);

  return 0; //Return something about the thread... tid?
}


/**
 * do_UserThreadExit erases and ends properly current thread
 */
void do_UserThreadExit(){

  delete currentThread->space; //TODO : A vÃ©rifier 
  currentThread->Finish();
}