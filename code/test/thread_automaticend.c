#include "syscall.h"

static void print(void *arg)
{
    char* phrase = (char*) arg;
    UserThreadCreate(print, phrase);    //ne doit pas marcher
    PutString(phrase);
}

int main()
{ 
    char* phrase = "hello coco\n";
    //UserThreadCreate((void*)phrase, phrase);  //TODO : ca fait planter, peut etre à gérer mais comment? Quoi qu'il a qu'a pas faire de cast...
    //int a = 1999999999;
    //UserThreadCreate(print, (char*)a);        //TODO : pareil
    UserThreadCreate(print, phrase);

    //Not reached
    return 0;
}