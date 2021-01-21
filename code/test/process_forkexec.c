#include "syscall.h"

int main() {
  char* file1 = "userpages0";
  char* file2 = "userpages1";
  
  ForkExec(file1);
  ForkExec(file2);

  End();
  return 0;
}