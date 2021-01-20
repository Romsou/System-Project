// userthread.h
//

#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"


/**
 * Create a new user thread and puts it in the ready list.
 * 
 * Create a new user thread and sets its function and arg attributes
 * so that we can jump to the code of this function later.
 * Also set ReturnAddrReg on the address of UserThreadExit() to 
 * automatically end the function f properly.
 * 
 * @param f: The function we want to create a user thread for.
 * @param arg: The argument we want to pass to f
 * @return: The id of the newly created user thread or -1 if the creation of the thread fails 
 */
extern int do_UserThreadCreate(int f, int arg);

/**
 * Erases and properly ends the current thread
 * 
 * As a side effect, this function frees all the threads
 * waiting for the current thread to end.
 */
extern void do_UserThreadExit();


/**
 * Allows a user thread to wait for the termination of another user thread.
 * 
 * This works by making the current thread take the semaphore of the one we want to wait for.
 * 
 * @param tid: The tid of the thread
 * @return: 0 on success
 */
extern int do_UserThreadJoin(int tid);

#endif //USERTHREAD_H