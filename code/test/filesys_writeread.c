#include "syscall.h"
//launch
/*

./nachos-step5 -f;
./nachos-step5 -cp filesys_writeread filesys_writeread;
./nachos-step5 -cp filesys_readwrite filesys_readwrite;
./nachos-step5 -x filesys_writeread;
./nachos-step5 -D;

*/

int main ()
{

    Create("outputtest");
    OpenFileId fileId = Open("outputtest");
    ForkExec("filesys_readwrite");

    char* buffer = "i'm writing something";
    int size = 22;
    Write(buffer, size, fileId);

    //wait, wait....
    // int i;
    //for(i = 0; i<100; i++)
      //  PutString("i'm blocking others\n");

    Close(fileId);
    End();
    /* not reached */
    return 0;
}