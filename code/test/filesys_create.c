#include "syscall.h"

int main ()
{
    Create("f1");
    OpenFileId fileId = Open("f1");
    char* buffer = "une chaine test";
    int size = 15;
    Write(buffer, size, fileId);
    Close(fileId);
    End();
    /* not reached */
    return 0;
}