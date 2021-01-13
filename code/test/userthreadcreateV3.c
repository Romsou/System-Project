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
    char* phrase = "Un thread\n";
    
    int i, nbThreads=0, nbThreadsError=0;
    for (i=0; i<NB_THREADS; i++) {
      if (UserThreadCreate(print, phrase) != -1) {
        nbThreads++;
      } else {
        nbThreadsError++;
      }
    }

    PutString("NbThreads : "); 
    PutInt(nbThreads);
    PutChar('\n');

    PutString("Expected NbThreads :");
    PutInt(NB_THREADS);
    PutChar('\n');

    PutString("NbThreads not created: ");
    PutInt(nbThreadsError);
    PutChar('\n');

    Halt();
    //Not reached
    return 0;
}