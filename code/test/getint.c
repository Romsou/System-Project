/* getint.c
 *	Simple program to test whether writing with getint works.
 *	
 *	Just do a "syscall" that read int on input.
 *
 */

#include "syscall.h"

int main ()
{
    int d = GetInt();
    PutInt(d);
    PutChar('\n');

    Halt ();

    /* not reached */
    return 0;
}
