#include "syscall.h"
//lauch
/*
*./nachos-final -f; ./nachos-final -cp filesys_create filesys_create; ./nachos-final -x filesys_create; ./nachos-final -D
*/

int main ()
{
	PutString("Test de creation, écriture, lecture et fermeture d'un fichier\n");
    Create("f1");
    OpenFileId fileId = Open("f1");
    char* buffer = "une chaine test";
    int size = 15;
    char res[size];
    
    Write(buffer, size, fileId);
    Close(fileId);

    fileId = Open("f1");
    Read(res,size,fileId);
    PutString("On a lu :\n");
    PutString(res);
    PutChar('\n');
    
    Close(fileId);
    End();
    /* not reached */
    return 0;
}