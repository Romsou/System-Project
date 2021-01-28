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
    for (i = 0; i < n; i++)
        PutChar(c+i);
    PutChar('\n');
}

int main ()
{
    print('a', 4);
    PutChar('1');
    PutChar('B');
    PutChar('\n');
    // PutChar(GetsChar());
    /* not reached */
    return 0;
}
