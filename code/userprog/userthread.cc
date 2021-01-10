// userthread.cc 
#include "syscall.h" 
#include "thread.h"
#include "system.h"
#include "machine.h"

struct forkArgs
{
  int func;
  int args;
};

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
static void StartUserThread(int f) {
  DEBUG('t',"Call of StartUserThread\n");
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  //machine->ReadRegister(StackReg);
  machine->WriteRegister(PCReg, f);
  //machine->Run();
}

/**
 * do_UserThreadCreate
 */
int do_UserThreadCreate(int f,int arg) {
  DEBUG('t',"Call of do_UserThreadCreate\n");
  struct forkArgs fArgs;
  fArgs.args = 1;
  fArgs.func = (int)&ThreadTestPrint;
  void *ptrfArgs = &fArgs;

  Thread *newThread = new Thread("new_user_thread");
  newThread->Fork(StartUserThread,(int)ptrfArgs);
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