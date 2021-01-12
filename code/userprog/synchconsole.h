#ifdef CHANGED
#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H
#include "copyright.h"
#include "utility.h"
#include "console.h"

class SynchConsole {
public:SynchConsole(char *readFile, char *writeFile);// initialize the hardware console device
	~SynchConsole();                    // clean up console emulation

	/**
	 * SynchPutChar allows to write, if possible, a character onto output.
	 * @param ch character to put.
	 */
	void SynchPutChar(const char ch);
	
	/**
	 * SynchGetChar allows to read, if possible, a character from input
	 * and return it.
	 * @return the readed character.
	 */
	char SynchGetChar();

	/**
	 * SynchPutString allows to write, if possible, a string of character 
	 * onto output.
	 * @param s string of character to put.
	 */
	void SynchPutString(const char *s);
	
	/**
	 * SynchGetString allows to read, if possible, a string of character from input
	 * and return it.
	 * @param s a char* to store readed string of char.
	 * @param n number of character to read.
	 */
	void SynchGetString(char *s, int n);

private:
	Console *console;
};

#endif // SYNCHCONSOLE_H
#endif // CHANGED