#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

//Semaphore in order to put in critical section any calls
static Semaphore *lockPutGet;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

/**
 * @class SynchConsole
 * 
 * @abstract SynchConsole is a class that provides synchronous I/O functions
 * 
 * @discussion SynchConsole works by encapsulating semaphores and call to the 
 * 			   Console class I/O methods, thus, providing synchronous I/O methods
 * 			   Moreover, the class contains several additional semaphores to allow
 * 			   concurrent calls by different user threads to the I/O methods.
 * 			   The output and input of SynchConsole are settled on readFile
 * 			   and writeFile respectively
 * 
 * @function SynchConsole
 * 		The constructor method of SynchConsole
 * 		@param readFile The input from which we read
 * 		@param writeFile The output to which we write
 * 
 * 
 * @function SynchPutChar
 * 		A function that provides a synchronous way to write a char on the output
 * 
 * @function SynchGetChar
 * 		A function that provides a synchronous way to get a char from input
 * 
 * @function SynchGetString
 * 		A function that provides a synchronous way to get a string from the input
 * 
 * @function SynchPutString
 * 		A function that provides a synchronous way to write a string on output
 * 
 * @var Console*
 */
SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);

	//Semaphore in order to put in critical section each calls for put and get
	lockPutGet = new Semaphore("lockPutGet", 1);

	console = new Console(readFile, writeFile, ReadAvail, WriteDone, 0);
}

/**
 * Dealloate synchconsole ,it associated console and all it semaphores.
 */
SynchConsole::~SynchConsole()
{
	delete console;

	delete writeDone;
	delete readAvail;

	delete lockPutGet;
}

/**
 * SynchPutChar allows to write, if possible, a character onto output.
 * @param ch character to put.
 */
void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar(ch);
	writeDone->P();
}

/**
 * SynchGetChar allows to read, if possible, a character from input
 * and return it.
 * @return the readed character.
 */
char SynchConsole::SynchGetChar()
{
	readAvail->P();
	return console->GetChar();
}

/**
 * SynchPutString allows to write, if possible, a string of character 
 * onto output.
 * @param s string of character to put.
 */
void SynchConsole::SynchPutString(const char s[])
{
	lockPutGet->P();

	int i;
	for (i = 0; s[i] != '\0'; i++)
		this->SynchPutChar(s[i]);

	lockPutGet->V();
}

/**
 * SynchGetString allows to read, if possible, a string of character from input
 * and return it.
 * @param s a char* to store readed string of char.
 * @param n number of character to read.
 */
void SynchConsole::SynchGetString(char *s, int n)
{
	ASSERT(s != NULL);
	ASSERT(n > 0);
	
	lockPutGet->P();

	int i = 0;

	s[0] = this->SynchGetChar();
	for (i = 1; i < n && s[i - 1] != EOF; i++)
		s[i] = this->SynchGetChar();

	s[i] = '\0';

	lockPutGet->V();
}