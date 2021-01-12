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
    int ret = UserThreadCreate(print, phrase);
    
    Halt();
    return ret;
}