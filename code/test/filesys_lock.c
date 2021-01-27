#include "syscall.h"
//launch
/*
*./nachos-step5 -f; ./nachos-step5 -cp filesys_lock filesys_lock;
		./nachos-step5 -x filesys_lock; ./nachos-step5 -D
*/

int main ()
{
    Create("f1");
    OpenFileId fileId = Open("f1");
    PutString("Apres 1 open...\n");
    OpenFileId fileId2 = Open("f1");
    PutString("Apres 2 open...\n");
    
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