#include "syscall.h"
//launch
/*
*./nachos-final -f; 
./nachos-final -cp filesys_lock filesys_lock; ./nachos-final -x filesys_lock; 
        ./nachos-final -D
*/

void print(void *j){
    OpenFileId fd = Open("f1");
    Close(fd);
    PutString("Libre !\n");
}

int main ()
{
    PutString("Test de la table utilisateur\n");
    Create("f1");

    int a = 1;
    int b = 2;
    int c = 3;
    int t1 = UserThreadCreate(print,&a);
    UserThreadCreate(print,&b);

    UserThreadJoin(t1);
    UserThreadCreate(print,&c);
    
    End();
    /* not reached */
    return 0;
}