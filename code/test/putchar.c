/* putchar.c
 *	Simple program to test whether writing with putchar works.
 *	
 *	Just do a "syscall" that write char on standard output.
 *
 */

#include "syscall.h"

void print(char c, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        PutChar(c+i);
    }
    PutChar('\n');
}


void read_and_write()
{
	char c = GetChar();
	PutChar('\n');
	PutChar(c);
	PutChar('\n');
}

int main ()
{
    //print('a', 4); //test for PutChar
    read_and_write();
    Halt ();

    /* not reached */
    return 0;
}
