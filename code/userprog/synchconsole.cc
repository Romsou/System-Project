#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

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

	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}
SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar(ch);
	writeDone->P();
}

//----------------------------------------------------------------
//Methode : SynchGetChar
//Quand un caractère peut être lu sur l'entrée
//il est lu puis retourné
//Author : Jah003
//----------------------------------------------------------------
char SynchConsole::SynchGetChar()
{
	char ch;

	readAvail->P();
	ch = console->GetChar();
	return ch;
}
void SynchConsole::SynchPutString(const char s[])
{
	int i;
	for(i = 0; s[i]!='\0'; i++) {
		SynchPutChar(s[i]);
	}
}
void SynchConsole::SynchGetString(char *s, int n)
{
	int i = 0;
	
	char ch = SynchGetChar();
	while(i<n && ch != EOF){
		*(s+i) = ch;
		ch = SynchGetChar();
		i++;
	}
}