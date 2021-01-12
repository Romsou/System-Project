#ifdef CHANGED
#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H
#include "copyright.h"
#include "utility.h"
#include "console.h"

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
class SynchConsole
{
public:
	SynchConsole(char *readFile, char *writeFile); // initialize the hardware console device
	~SynchConsole();							   // clean up console emulation

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