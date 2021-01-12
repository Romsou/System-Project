// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"

#define MAX_LEN_INT 11
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC()
{
  int pc = machine->ReadRegister(PCReg);
  machine->WriteRegister(PrevPCReg, pc);
  pc = machine->ReadRegister(NextPCReg);
  machine->WriteRegister(PCReg, pc);
  pc += 4;
  machine->WriteRegister(NextPCReg, pc);
}

/**
 * Copy a string from our MIPS machine to the Linux host system
 * 
 * @param from: The MIPS pointer as an integer from which we retrieve the string
 *              to be copied
 * @param to: A pointer to a string to which we want to copy
 * @param size: The maximum number of characters we want to copy
 */
void copyStringFromMachine(int from, char *to, unsigned size)
{
  ASSERT(from >= 0);
  ASSERT(to != NULL);

  if (size == 0)
    return;

  int valRead;
  unsigned int i;

  for (i = 0; i < size && machine->ReadMem(from + i, 1, &valRead); i++)
    to[i] = valRead;

  ASSERT(i == size);
  to[i] = '\0';
}

/**
 * Copy a string from the Linux host system to our MIPS machine
 * 
 * @param from: A pointer to a string from which we retrieve the string
 *              to be copied
 * @param to: The MIPS pointer as an integer to which we want to copy
 * @param size: The maximum number of characters we want to copy
 */
void copyStringToMachine(char *from, int to, unsigned size)
{
  ASSERT(to >= 0);
  ASSERT(from != NULL);

  if (size == 0)
    return;

  unsigned int i;

  for (i = 0; i < size && machine->WriteMem(to + i, 1, from[i]); i++)
    ;

  ASSERT(i == size);
}

/**
 * handleHalt Handles the Halt() system call
 */
void handleHalt()
{
  DEBUG('a', "Shutdown, initiated by user program.\n");
  interrupt->Halt();
}

/**
 * handleError Handles the cases in which a system calls fails
 */
void handleError(ExceptionType which, int type)
{
  printf("Unexpected user mode exception %d %d\n", which, type);
  ASSERT(FALSE);
}

/**
  * Handles the call system PutChar
  * no param, read the argument in register r4
  */
void handlePutChar()
{
  DEBUG('a', "Interruption for putchar, raised by syscall\n");
  synchconsole->SynchPutChar((char)machine->ReadRegister(4));
}

/**
 * handleGetChar handles SC_GetChar system call. Get a char
 * from synchconsole.
 */
void handleGetChar()
{
  DEBUG('a', "Interruption for getchar, raised by syscall\n");
  char c = synchconsole->SynchGetChar();
  machine->WriteRegister(2, (int)c);
}

/**
 * Handles the call system End
 * puts end to the user function main
 * print some information about the process
 */
void handleEnd()
{
  DEBUG('a', "Interruption for end of process\n");
  int ad = machine->ReadRegister(37);
  printf("Clean exit with that address %d\n", ad);
  machine->WriteRegister(2, ad);
  interrupt->Halt();
}

//----------------------------------------------------------------------
// handlePutString : Handler for system call SC_PutString. Put a given
// String into synchConsole.
//----------------------------------------------------------------------
void handlePutString()
{
  DEBUG('a', "PutString.\n");
  char s[MAX_STRING_SIZE];
  copyStringFromMachine(machine->ReadRegister(4), s, MAX_STRING_SIZE);
  synchconsole->SynchPutString(s);
}

/**
 * handleGetString handles SC_GetString system call. Get a string
 * of char from synchconsole.
 */
void handleGetString()
{
  DEBUG('a', "GetString.\n");
  char s[MAX_STRING_SIZE];
  int size = machine->ReadRegister(5);

  synchconsole->SynchGetString(s, size);
  copyStringToMachine(s, machine->ReadRegister(4), size);
}

/**
 * handlePutInt handles SC_PutIn system call. Put a given
 * Int into synchConsole.
 */
void handlePutInt()
{
  DEBUG('a', "PutIn.\n");
  char s[MAX_LEN_INT];
  snprintf(s, MAX_LEN_INT, "%d", machine->ReadRegister(4));
  synchconsole->SynchPutString(s);
}

/**
 * handleGetInt handles SC_GetInt system call. Get an Int from
 * synchConsole.
 */
void handleGetInt()
{
  DEBUG('a', "GetInt.\n");

  char s[MAX_LEN_INT];

  int i = 0;
  char ch = synchconsole->SynchGetChar();
  while (i < MAX_LEN_INT - 1 && ch >= '0' && ch <= '9' && ch != EOF && ch != '\n' && ch != '\t')
  {
    s[i] = ch;
    ch = synchconsole->SynchGetChar();
    i++;
  }
  s[i] = '\0';

  int d = 0;
  sscanf(s, "%d", &d);
  machine->WriteRegister(2, d);
}

void handleUserThreadCreate()
{
  DEBUG('t', "Call for creating user thread\n");
  //Retrieve f and arg here and pass them to DoUserThreadCreate
  int f = machine->ReadRegister(4);
  int arg = machine->ReadRegister(5);

  int retval = do_UserThreadCreate(f, arg);

  machine->WriteRegister(2, retval);
  
}

/**
 * handleUserThreadExit
 */
void handleUserThreadExit()
{
  do_UserThreadExit();
}

void handleUserThreadJoin()
{
  int id = machine->ReadRegister(4);
  int retval = do_UserThreadJoin(id);
  machine->WriteRegister(2, retval);
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
  int type = machine->ReadRegister(2);

#ifndef CHANGED // Noter le if*n*def
  if ((which == SyscallException) && (type == SC_Halt))
  {
    DEBUG('a', "Shutdown, initiated by user program.\n");
    interrupt->Halt();
  }
  else
  {
    printf("Unexpected user mode exception %d %d\n", which, type);
    ASSERT(FALSE);
  }
#else // CHANGED

  if (which == SyscallException)
  {
    switch (type)
    {
    case SC_Halt:
      handleHalt();
      break;
    case SC_PutChar:
      handlePutChar();
      break;
    case SC_PutString:
      handlePutString();
      break;
    case SC_GetChar:
      handleGetChar();
      break;
    case SC_UserThreadExit:
      handleUserThreadExit();
      break;
    case SC_End:
      handleEnd();
      break;
    case SC_PutInt:
      handlePutInt();
      break;
    case SC_GetInt:
      handleGetInt();
      break;
    case SC_GetString:
      handleGetString();
      break;
    case SC_UserThreadCreate:
      handleUserThreadCreate();
      break;
    case SC_UserThreadJoin:
      handleUserThreadJoin();
      break;
    default:
      handleError(which, type);
      break;
    }
  }
  else
    handleError(which, type);

#endif
  // LB: Do not forget to increment the pc before returning!
  UpdatePC();
  // End of addition
}