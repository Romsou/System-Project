#include "syscall.h"

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