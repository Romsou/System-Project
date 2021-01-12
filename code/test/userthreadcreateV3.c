#include "syscall.h"

#define NB_THREADS 100

static void print(void *arg)
{
    char* phrase = (char*) arg;
    PutString(phrase);
    UserThreadExit();
}

int main()
{
    char* phrase = "hello coco\n";
    
    int i, nbThreads;
    for (i=0; i<NB_THREADS; i++) {
      if (UserThreadCreate(print, phrase) != -1) {
        nbThreads++;
      }
    }

    PutString("NbThreads : "); 
    PutInt(nbThreads);
    PutChar('\n');

    Halt();
    //Not reached
    return 0;
}