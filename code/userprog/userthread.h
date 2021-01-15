// userthread.h
//

#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"

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

/**
 * Indicates whether the list of user threads is empty
 * 
 * @return A boolean indicating whether the list of user thread is empty
 */
extern bool isEmptyListOfUserThreads();

/**
 * Creates a new user thread and puts it in the ready list.
 * 
 * @param f: The function we want to create a user thread for.
 * @param arg: The argument we want to pass to f
 * @return: thread id or -1 if the creation of the thread fails 
 */
extern int do_UserThreadCreate(int f, int arg);

/**
 * Properly removes the current thread from ListOfUserThreads.
 */
extern void DeleteThreadFromList();

/**
 * do_UserThreadExit erases and properly ends the current thread
 */
extern void do_UserThreadExit();

/**
 * Indicate to the current thread to wait for the thread
 * identified by "tid"
 * 
 * @param tid: The id of the thread we want to wait;
 */
extern int do_UserThreadJoin(int tid);

#endif //USERTHREAD_H