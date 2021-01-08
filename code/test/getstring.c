/* getstring.c
 *	Simple program to test whether writing with getString works.
 *	

 *	Just do a "syscall" that read string on intput.
 *
 */

#include "syscall.h"

int main ()
{
    int size = 10;
    char s[size];
    GetString(s, size);
    PutString(s);
    PutChar('\n');

    Halt ();
    /* not reached */
    return 0;
}

