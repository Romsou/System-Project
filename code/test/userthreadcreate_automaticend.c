#include "syscall.h"

static void print(void *arg)
{
    char* phrase = (char*) arg;
    //UserThreadCreate(print, phrase); //est-ce que cette fonction a un sens ici ?
    PutString(phrase);
}

int main()
{
    char* phrase = "hello coco\n";
    UserThreadCreate(print, phrase);

    
  
    //End();
    //Not reached
    return 0;
}