// thread.h
//      Data structures for managing threads.  A thread represents
//      sequential execution of code within a program.
//      So the state of a thread includes the program counter,
//      the processor registers, and the execution stack.
//
//      Note that because we allocate a fixed size stack for each
//      thread, it is possible to overflow the stack -- for instance,
//      by recursing to too deep a level.  The most common reason
//      for this occuring is allocating large data structures
//      on the stack.  For instance, this will cause problems:
//
//              void foo() { int buf[1000]; ...}
//
//      Instead, you should allocate all data structures dynamically:
//
//              void foo() { int *buf = new int[1000]; ...}
//
//
//      Bad things happen if you overflow the stack, and in the worst
//      case, the problem may not be caught explicitly.  Instead,
//      the only symptom may be bizarre segmentation faults.  (Of course,
//      other problems can cause seg faults, so that isn't a sure sign
//      that your thread stacks are too small.)
//
//      One thing to try if you find yourself with seg faults is to
//      increase the size of thread stack -- ThreadStackSize.
//
//      In this interface, forking a thread takes two steps.
//      We must first allocate a data structure for it: "t = new Thread".
//      Only then can we do the fork: "t->fork(f, arg)".
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef THREAD_H
#define THREAD_H

#include "copyright.h"
#include "utility.h"
#include "filetable.h"

#ifdef USER_PROGRAM
#include "machine.h"
#include "addrspace.h"
#include "synch.h"
#endif

// CPU register state to be saved on context switch.
// The SPARC and MIPS only need 10 registers, but the Snake needs 18.
// For simplicity, this is just the max over all architectures.
#define MachineStateSize 18

// Size of the thread's private execution stack.
// WATCH OUT IF THIS ISN'T BIG ENOUGH!!!!!
#define StackSize (4 * 1024) // in words

// Thread state
enum ThreadStatus
{
  JUST_CREATED,
  RUNNING,
  READY,
  BLOCKED
};

// external function, dummy routine whose sole job is to call Thread::Print
extern void ThreadPrint(int arg);

/**
 * @struct FunctionAndArgs
 * 
 * A structure used to pass the address of our function
 * and its arguments to StartUserThread in userthread.cc
 * 
 * @field func The address of the function we want to pass as a parameter
 * @field args The address of the first argument to the function we want to
 *             make a thread for.
 * @field end Address of UserThreadExit function, to allow us to automatically
 *            exit the thread once it's done.
 */
struct FunctionAndArgs
{
  int func;
  int args;
  int returnAddr;
};

// The following class defines a "thread control block" -- which
// represents a single thread of execution.
//
//  Every thread has:
//     an execution stack for activation records ("stackTop" and "stack")
//     space to save CPU registers while not running ("machineState")
//     a "status" (running/ready/blocked)
//
//  Some threads also belong to a user address space; threads
//  that only run in the kernel have a NULL address space.
class Thread
{
private:
  // NOTE: DO NOT CHANGE the order of these first two members.
  // THEY MUST be in this position for SWITCH to work.
  int *stackTop;                      // the current stack pointer
  int machineState[MachineStateSize]; // all registers except for stackTop
                                      // some of the private data for this class is listed above

  int *stack; // Bottom of the stack NULL if this is the main thread (If NULL, don't deallocate stack)
  const char *name;

  Thread *parent;
  ThreadStatus status; // ready, running or blocked

  int id; // id of our UserThread
  int pid;
  int ppid;
  int index; // index in the Thread array use in addrSpace

  Semaphore *waitQueue;
  int numOfWaitingThreads;
  struct FunctionAndArgs *functionAndArgs;
  
  FileTable *openedThreadFiles;

  // A thread running a user program actually has *two* sets of CPU registers --
  // one for its state while executing user code, one for its state
  // while executing kernel code.
  int userRegisters[NumTotalRegs]; // user-level CPU register state
  
  // Allocate a stack for thread. Used internally by Fork()
  void StackAllocate(VoidFunctionPtr func, int arg);

public:
  Thread(const char *debugName);
  ~Thread(); // NOTE -- thread being deleted must not be running when delete is called

  AddrSpace *space; // User code this thread is running.

  static int userThreadCount;
  static int processCount;

  int wakeUpTime;
  bool signaled;

  // basic thread operations
  void Fork(VoidFunctionPtr func, int arg); // Make thread run (*func)(arg)
  void Yield();                             // Relinquish the CPU if any other thread is runnable
  void Sleep();                             // Put the thread to sleep and relinquish the processor
  void Finish(); // The thread is done executing
 
  void CheckOverflow(); // Check if thread has overflowed its stack 
  void Print();

  void SaveUserState();    // save user-level register state
  void RestoreUserState(); // restore user-level register state

  void TemporarilySleep();

  const char *getName();
  void setStatus(ThreadStatus st);

  int getTid();
  void setTid(int i);
  int generateTid();

  int getIndex();
  void setIndex(int i);

  void waitThread();
  void clearWaitingThreads();
  int getNumberOfWaitingThreads();

  int getFunction();
  void setFunction(int f);

  int getArgs();
  void setArgs(int args);

  int getReturnAddr();
  void setReturnAddr(int returnAddr);

  int generatePid();
  int getPid();
  void setPid(int ProcessId);

  int getPpid();
  void setPpid(int ParentProcessId);

  FileTable *getFileTable();
};

// Magical machine-dependent routines, defined in switch.s
extern "C"
{
  // First frame on thread execution stack;
  //      enable interrupts
  //      call "func"
  //      (when func returns, if ever) call ThreadFinish()
  void ThreadRoot();

  // Stop running oldThread and start running newThread
  void SWITCH(Thread *oldThread, Thread *newThread);
}

#endif // THREAD_H