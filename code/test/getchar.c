#include "syscall.h"

void read_and_write()
{
    for(;;)
    {
        char c = GetChar();
        if(c=='q') {break;}
        PutChar(c);
        
    }
    PutChar('\n');
}

int main ()
{
    read_and_write();
    //Halt ();

    /* not reached */
    return 0;
}