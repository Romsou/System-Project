#include "syscall.h"

//Test for a function withouth arguments
void countDown(void* arg)
{
    int i;
    for (i = 10; i >= 0; i--)
    {
        PutInt(i);
        PutChar('\n');
    }
    PutChar('\n');
}

int main()
{
    PutString("Début du main...\n");
    int ret1 = UserThreadCreate(countDown, 0);
    int ret2 = UserThreadCreate(countDown,0);
    UserThreadJoin(ret1);
    UserThreadJoin(ret2);

    Halt();
    //Not reached
    return 0;
}
