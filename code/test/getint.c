/* putchar.c
 *	Simple program to test whether writing with putchar works.
 *	
 *	Just do a "syscall" that write char on standard output.
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
