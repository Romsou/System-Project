// userthread.h
//      

#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"

/**
 * A structure use to pass the address of our function
 * and its arguments to StartUserThread in userthread.cc
 */
struct FunctionAndArgs
{
  int func; 
  int args;
};

/**
 * Return true if all users threads are properly ends,with an 
 * UserThreadExit() call. Else, return false.
 * @return a boolean, true if lsit of userThread is empty.
 */
extern bool isEmptyListOfUserThreads();

/**
 * Create a new user thread and puts it in the ready list.
 * 
 * @param f: The function we want to create a user thread for.
 * @param arg: The argument we want to pass to f
 * @return: 0 or -1 if the creation of the thread fails 
 */
extern int do_UserThreadCreate(int f,int arg);

/**
 * Remove properly current thread from ListOfUserThreads.
 */
extern void DeleteThreadFromList();

/**
 * do_UserThreadExit erases and ends properly current thread
 */
extern void do_UserThreadExit();

/**
 * do_UserThreadJoin
 */
extern int do_UserThreadJoin(int tid);

#endif //USERTHREAD_H