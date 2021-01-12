
#include "syscall.h"

void print(void *j){
	int i = *(int *)j;
	//PutString("L'entier est : ");
	//PutInt(i);
	//PutChar('\n');
	if(i%2)
		PutString("Je suis un nombre impair\n");
	else
		PutString("Je suis un nombre pair\n");

	UserThreadExit();
}

int main(){
	PutString("Debut du main...\n");
	int a = 1, b=2, c=3, d=4, e=5, f=6;
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
	UserThreadCreate(print,&c);
	UserThreadCreate(print,&d);
	UserThreadCreate(print,&e);
	UserThreadCreate(print,&f);
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);

	Halt();
	//Not reached
	return 0;
}