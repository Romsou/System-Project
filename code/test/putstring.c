/* putstring.c
 *	Simple program to test whether writing with putString works.
 *	
 *	Just do a "syscall" that write string on standard output.
 *
 */

#include "syscall.h"

void print(char *s)
{
  PutString(s);
}

int main ()
{
    print("abcdefgh\n");
    Halt ();

    /* not reached */
    return 0;
}
