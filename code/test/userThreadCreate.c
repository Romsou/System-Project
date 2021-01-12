#include "syscall.h"

//Test for a function withouth arguments
void countDown(void* arg)
{
    int i;
    for (i = 100; i >= 0; i--)
    {
        PutChar(i - ((int)'0'));
    }
    PutChar('\n');
    UserThreadExit();
}

int main()
{
    PutString("Début du main...\n");
    UserThreadCreate(countDown, 0);
    UserThreadCreate(countDown,0);
    return 0;
}
