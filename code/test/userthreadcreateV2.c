#include "syscall.h"

static void print(void *arg)
{
    char* phrase = (char*) arg;
    PutString(phrase);
    UserThreadExit();
}

int main()
{
    char* phrase = "hello coco\n";
    UserThreadCreate(print, phrase);
    
    Halt();
    //Not reached
    return 0;
}