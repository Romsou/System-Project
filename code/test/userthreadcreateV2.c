#include "syscall.h"

static void print() 
{
   PutString("Baba");
}

int main() 
{
    int ret = UserThreadCreate(print, 0);
    //PutString("Threadmain");
    //UserThreadExit(print);
    UserThreadJoin(ret);
    return ret;
}