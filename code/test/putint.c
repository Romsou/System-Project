/* putinit.c
 *	Simple program to test whether writing with putint works.
 *	
 *	Just do a "syscall" that write int on standard output.
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
    PutInt(GetInt());
    PutChar('\n');

    End();
    /* not reached */
    return 0;
}
