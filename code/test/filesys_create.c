#include "syscall.h"
//lauch
/*
*./nachos-step5 -f; ./nachos-step5 -cp filesys_create filesys_create;
		./nachos-step5 -x filesys_create; ./nachos-step5 -D
*/

int main ()
{
	PutString("Debut du main...\n");
    Create("f1");
    OpenFileId fileId = Open("f1");
    PutString("Apres open...\n");
    char* buffer = "une chaine test";
    int size = 15;
    char res[size];
    
    PutString("Avant write\n");
    Write(buffer, size, fileId);
    Close(fileId);


    PutString("Avant read\n");
    fileId = Open("f1");
    Read(res,size,fileId);
    PutString(res);
    
    Close(fileId);
    End();
    /* not reached */
    return 0;
}