#include "syscall.h"

/*
  This test needs files : process_forkexec, userpages0, userpages1, putchar
*/
void print(void *arg){
  char* phrase = (char*) arg;
  PutString(phrase);
  ForkExec("userpages0");   // ne devrait pas marcher
}

int main() {
  char* file1 = "userpages0";
  char* file2 = "userpages1";
  ForkExec(file1);
  ForkExec(file2);

  char* phrase = "Je suis la phrase\n";
  PutString("Création des userThread\n");
  UserThreadCreate(print, phrase);
  UserThreadCreate(print, phrase);
  UserThreadCreate(print, phrase);
  PutString("Fin du test\n");
  return 0;
}