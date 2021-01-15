#include "syscall.h"

static void print(void *arg)
{
    char* phrase = (char*) arg;
    PutString(phrase);
}

int main()
{
    char* phrase = "hello coco\n";
    UserThreadCreate(print, phrase);
  
    //Not reached
    return 0;
}