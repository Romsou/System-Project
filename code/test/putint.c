/* putchar.c
 *	Simple program to test whether writing with putchar works.
 *	
 *	Just do a "syscall" that write char on standard output.
 *
 */

#include "syscall.h"

void print(int n)
{
    PutInt(n);
    PutChar('\n');
}

int main ()
{
    print(1234567891);
    Halt ();

    /* not reached */
    return 0;
}
