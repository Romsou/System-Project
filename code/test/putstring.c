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
    //less than 64 char
    print("abcdefghijkl 123456 ¤¤¤¤¤¤\n");

    //more than 64 char
    print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    
    //2 bytes char
    print("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤");
    
    PutChar('\n');
    
    //char s[64];
    //GetString(s, 64);
    //PutString(s);
    
    End();
    /* not reached */
    return 0;
}
