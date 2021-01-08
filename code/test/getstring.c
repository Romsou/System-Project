/* getstring.c
 *	Simple program to test whether writing with getString works.
 *	
 *	Just do a "syscall" that write string on standard output.
 *
 */

#include "syscall.h"


void test1(char *s, int n)
{
  GetString(s,n);
}

int main ()
{
    char string[10];
    int n = 2;
    test1(string,n);
    string[0] = '\0';
    PutString(string);

    Halt ();

    /* not reached */
    return 0;
}

