#include "syscall.h"
//launch
/*
./nachos-step5 -f; ./nachos-step5 -cp filesys_userthread filesys_userthread;./nachos-step5 -x filesys_userthread;./nachos-step5 -D
*/

#define NB_THREADS 10

char* name = "LeFichierTest";

static void handleFile(void *arg)
{
  int id = Open(name);
  char* buffer = "coucou c moi\n";
  Write(buffer, 14, id);
  Close(id);
}

int main()
{
  Create(name);
  int i;
    for (i=0; i<NB_THREADS; i++) {
     UserThreadCreate(handleFile,0);
    }
  End();
  return 0;
}