#include "syscall.h"
//launch
/*
*./nachos-final -f; ./nachos-final -cp filesys_lock filesys_lock;
		./nachos-final -x filesys_lock; ./nachos-final -D
*/

int main ()
{
    Create("f1");
    OpenFileId fileId = Open("f1");
    OpenFileId fileId2 = Open("f1");
    
    char* buffer = "une chaine test";
    int size = 15;
    char res[size];
    
    Write(buffer, size, fileId);
    Close(fileId);


    PutString("Avant read\n");
    Read(res,size,fileId2);
    PutString(res);
    
    Close(fileId);
    End();
    /* not reached */
    return 0;
}