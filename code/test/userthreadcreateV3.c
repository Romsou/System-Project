#include "syscall.h"

#define NB_THREADS 8

static void print(void *arg)
{
    char* phrase = (char*) arg;
    PutString(phrase);
}

int main()
{
    char* phrase = "Un thread\n";
    
    int i, nbThreads;
    for (i=0; i<NB_THREADS; i++) {
      if (UserThreadCreate(print, phrase) != -1) {
        nbThreads++;
      }
    }

    PutString("NbThreads : "); 
    PutInt(nbThreads);
    PutChar('\n');

    PutString("Expected NbThreads : "); 
    PutInt(NB_THREADS);
    PutChar('\n');

    Halt();
    //Not reached
    return 0;
}