#include "syscall.h"

int main() {
  //char* file1 = "userpages0";
  char* file2 = "putchar";
  ForkExec("putstring");
  ForkExec(file2);

  //End();
  return 0;
}