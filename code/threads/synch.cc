// synch.cc
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(const char *debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------

void Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

    while (value == 0)
    {                                         // semaphore not available
        queue->Append((void *)currentThread); // so go to sleep
        currentThread->Sleep();
    }
    value--; // semaphore available,
    // consume its value

    (void)interrupt->SetLevel(oldLevel); // re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------

void Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL) // make thread ready, consuming the V immediately
        scheduler->ReadyToRun(thread);
    value++;
    (void)interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments
// Note -- without a correct implementation of Condition::Wait(),
// the test case in the network assignment won't work!
Lock::Lock(const char *debugName)
{
    name = debugName;
    internalLock = new Semaphore(debugName, 1);
    state = FREE;
    ownerId = -1;
}

Lock::~Lock()
{
    delete internalLock;
}

void Lock::Acquire()
{
    internalLock->P();
    state = BUSY;
    ownerId = currentThread->getTid();
    DEBUG('d', "Lock %s was acquired\n", getName());
}

void Lock::Release()
{
    internalLock->V();
    state = FREE;
    ownerId = -1;
    DEBUG('d', "Lock %s was freed\n", getName());
}

bool Lock::isHeldByCurrentThread()
{
    return currentThread->getTid() == ownerId;
}

Condition::Condition(const char *debugName)
{
    queue = new List();
}

Condition::~Condition()
{
    delete queue;
}
void Condition::Wait(Lock *lock)
{
    // Releases the lock so that another thread can enter its critic section
    conditionLock->Release();
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

    queue->Append((void *)currentThread);
    currentThread->Sleep();
    // Attempt to reacquire it once awoken.

    conditionLock->Acquire();
    (void)interrupt->SetLevel(oldLevel);
}

void Condition::Signal(Lock *lock)
{
    Thread *thread;
    // Must check it the current thread owns the lock to avoid
    // undefined behaviors.
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

    if (conditionLock->isHeldByCurrentThread())
    {
        thread = (Thread *)queue->Remove();
        if (thread != NULL)
            scheduler->ReadyToRun(thread);
    }
    (void)interrupt->SetLevel(oldLevel);
}

void Condition::Broadcast(Lock *lock)
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

    while ((thread = (Thread *)queue->Remove()) != NULL)
        scheduler->ReadyToRun(thread);
    (void)interrupt->SetLevel(oldLevel);
}

static void watchDog(int arg)
{
    Condition* cond = (Condition*) arg;
    int time = cond->getTime();

    for (int i = 0; i < time; i++)
        ;

    cond->setTimeout();
    cond->Signal(cond->getLock());
}

int Condition::temporaryWait(int , Lock* lock)
{
    this->limitTime = limitTime;
    this->conditionLock = lock;

    Thread* watchDogThread = new Thread("Watch dog");
    watchDogThread->Fork(watchDog, (int) this);
    this->Wait(lock);
   
    if(timeout)
    {
        resetTimeout();
        return -1;
    }

    return 0;
}

void Condition::setTimeout()
{
    timeout = true;
}

void Condition::resetTimeout()
{
    timeout = false;
}

Lock* Condition::getLock()
{
    return conditionLock;
}

int Condition::getTime()
{
    return limitTime;
}
