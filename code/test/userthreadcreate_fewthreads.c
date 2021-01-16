
#include "syscall.h"

void print(void *j){
	int i = *(int *)j;
	PutString("L'entier est : ");
	PutInt(i);
	PutString("\n");
	if(i%2)
		PutString("Je suis un nombre impair\n");
	else
		PutString("Je suis un nombre pair\n");
}

int main(){
	PutString("Test sur la création et l'ordonnancement des threads\n");

	int a = 1, b=2, c=3, d=4, e=5, f=6;
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
	UserThreadCreate(print,&c);
	UserThreadCreate(print,&d);
	UserThreadCreate(print,&e);
	UserThreadCreate(print,&f);
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);

	End();
	//Not reached
	return 0;
}