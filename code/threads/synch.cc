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
    blockedThreads = new List();
    conditionLock = new Lock("Condition lock");
}

Condition::~Condition()
{
    delete blockedThreads;
    delete conditionLock;
}

int Condition::temporaryWait(int timeToWait, Lock *lock)
{
    currentThread->signaled = false;
    currentThread->wakeUpTime = stats->totalTicks + timeToWait;
    DEBUG('t', "Put to sleep at time: %ld\n", currentThread->wakeUpTime - timeToWait);
    Wait(lock);
    if (currentThread->signaled)
        return 1;
    else
        return 0;
}

void Condition::Wait(Lock *lock)
{
    // Makes the lock release and the sleeping atomic.
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    lock->Release();
    blockedThreads->Append((void *)currentThread);
    currentThread->TemporarilySleep();

    (void)interrupt->SetLevel(oldLevel);

    lock->Acquire();
}

void Condition::Signal(Lock *lock)
{
    DEBUG('t', "Signaling thread");

    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    Thread *thread;
    // Must check it the current thread owns the lock to avoid
    // undefined behaviors.
    if (lock->isHeldByCurrentThread())
    {
        thread = (Thread *)blockedThreads->Remove();
        if (thread != NULL)
        {
            DEBUG('t', "Signaling thread %s", thread->getName());
            thread->signaled = true;
            scheduler->WakeUpReadyThreads();
        }
    }

    (void)interrupt->SetLevel(oldLevel);
}

void Condition::Broadcast(Lock *lock)
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // disable interrupts

    if (lock->isHeldByCurrentThread())
        while (!blockedThreads->IsEmpty())
            Signal(lock);

    (void)interrupt->SetLevel(oldLevel);
}

Lock *Condition::getLock()
{
    return conditionLock;
}