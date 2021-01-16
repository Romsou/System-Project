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
    PutString("Test sur l'attente de fin d'execution des threads\n");
    int ret1 = UserThreadCreate(countDown, 0);
    UserThreadJoin(ret1);
    int ret2 = UserThreadCreate(countDown,0);
    UserThreadJoin(ret2);

    End();
    //Not reached
    return 0;
}
