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

void puts(void *s)
{
	char *p = (char *)s;
    PutString(p);

	/*for(p=(char*)s ; *p != '\0' ; p++){
		PutChar(*p);
	}*/
}

int main()
{
	char* phrase = "abc\n";
    PutString("Test sur l'attente de fin d'execution des threads\n");

    int ret1 = UserThreadCreate(countDown, 0);
    UserThreadJoin(ret1);
    int ret2 = UserThreadCreate(countDown,0);
    UserThreadJoin(ret2);

	UserThreadCreate(puts,phrase);
	UserThreadCreate(puts,phrase);
	UserThreadCreate(puts,phrase);
	UserThreadCreate(puts,phrase);

    End();
    //Not reached
    return 0;
}
