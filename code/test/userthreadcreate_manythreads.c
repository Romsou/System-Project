#include "syscall.h"

#define NB_THREADS 100

static void print(void *arg)
{
    //char* phrase = (char*) arg;
    //PutString(phrase);
    PutChar('a');
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
      int a =1 + 6 * 8 - 2 / 7 + 6 / 7 - 9;
      int b = 9 * 7 / 2 - 6 + 3 + 8 + 5;
      b = a * 8 - 999 + 6 / 3;
      a = b + a - 88 / 2 * 3;
      a = 999 * b - 88 / 7;
      b = a / 7;
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