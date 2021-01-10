#include "machine.h"
#include "syscall.h"
#include "system.h"
#include "addrspace.h"
#include "userthread.h"

static void StartUserThread(int f) {
  currentThread->space->InitRegisters();
  currentThread->space->RestoreState();

  //machine->ReadRegister(StackReg);
  machine->WriteRegister(PCReg, f);
  machine->Run();
}

int do_UserThreadCreate(int f, int arg)
{
    Thread *newThread = new Thread("new_user_thread");
    newThread->Fork(StartUserThread, f);

    //thread can't be created
    if (newThread == NULL)
        return -1;
}