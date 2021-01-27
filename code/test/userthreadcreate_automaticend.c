#include "syscall.h"

static void print(void *arg)
{
    char* phrase = (char*) arg;
    UserThreadCreate(print, phrase);
    PutString(phrase);
}

int main()
{
    char* phrase = "hello coco\n";
    UserThreadCreate(print, phrase);
    End();
    //Not reached
    return 0;
}