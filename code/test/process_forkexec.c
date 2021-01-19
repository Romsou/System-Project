#include "syscall.h"

int main() {
  char* file1 = "putstring";
  char* file2 = "putchar";
  ForkExec(file1);
  ForkExec(file2);

  //End();
  return 0;
}