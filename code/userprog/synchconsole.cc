#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

//Semaphore in order to put in critical section any calls
static Semaphore *lockPutChar;
static Semaphore *lockGetChar;
static Semaphore *lockPutString;
static Semaphore *lockGetString;


static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

//-------------------------------------------------------
//Class SynchConsole émule class Console
//Cache la presence de sémaphore
//Constructeur :
// Attributs : deux sémaphores readAvail et writeDone
// similaires à ceux utilisés dans progtest.cc
// et un console de la classe Console
// Argument : readFile et writeFile 2 fichier en lecture 
// et ecriture, si NULL on prend stdin et stdout
// servent à construire console comme ReadAvail et WriteDone
// les deux fonctions definies en static
//-------------------------------------------------------
SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);

	//Semaphore in order to put in critical section each calls for put and get
	lockPutChar = new Semaphore("lockPutChar", 1);
	lockGetChar = new Semaphore("lockGetChar", 1);
	lockPutString = new Semaphore("lockPutString", 1);
	lockGetString = new Semaphore("lockGetString", 1);

	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}
SynchConsole::~SynchConsole()
{
	delete console;
	
	delete writeDone;
	delete readAvail;

	delete lockPutChar;
	delete lockGetChar;
	delete lockPutString;
	delete lockGetString;

}

/**
 * SynchPutChar allows to write, if possible, a character onto output.
 * @param ch character to put.
 */
void SynchConsole::SynchPutChar(const char ch)
{
	lockPutChar->P();

	console->PutChar(ch);
	writeDone->P();

	lockPutChar->V();
}

/**
 * SynchGetChar allows to read, if possible, a character from input
 * and return it.
 * @return the readed character.
 */
char SynchConsole::SynchGetChar()
{
	lockGetChar->P();
	
	char ch;
	readAvail->P();
	ch = console->GetChar();
	return ch;

	lockGetChar->V();
}

/**
 * SynchPutString allows to write, if possible, a string of character 
 * onto output.
 * @param s string of character to put.
 */
void SynchConsole::SynchPutString(const char s[])
{
	lockPutString->P();

	int i;
	for(i = 0; s[i]!='\0'; i++) {
		SynchPutChar(s[i]);
	}

	lockPutString->V();
}

/**
 * SynchGetString allows to read, if possible, a string of character from input
 * and return it.
 * @param s a char* to store readed string of char.
 * @param n number of character to read.
 */
void SynchConsole::SynchGetString(char *s, int n)
{
	lockGetString->P();

	int i = 0;
	
	char ch = SynchGetChar();
	while(i<n && ch != EOF){
		*(s+i) = ch;
		ch = SynchGetChar();
		i++;
	}
	s[i] = '\0';

	lockGetString->V();
}