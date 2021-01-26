#include "syscall.h"
//launch
/*
./nachos-step5 -f; ./nachos-step5 -cp filesys_create filesys_create;./nachos-step5 -x filesys_create;./nachos-step5 -D
*/

int main ()
{
    char* bufferWrite = "coucou helllllo";
    int size = 15;
    
    Create("f1");
    OpenFileId fileId = Open("f1");
    Write(bufferWrite, size, fileId);
    Close(fileId);
    
    //Try to read
    fileId = Open("f1");
    char bufferRead[size];
    Read(bufferRead, size, fileId);
    PutString(bufferRead);
    Close(fileId);

    End();
    /* not reached */
    return 0;
}