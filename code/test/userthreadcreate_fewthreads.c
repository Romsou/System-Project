
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

	int a=11;
	int b=22;
	int c=33;
	int d=44;
	int e=55; 
	int f=66;
	int g=77;
	int h=88;

	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
	UserThreadCreate(print,&c);
	UserThreadCreate(print,&d);
	UserThreadCreate(print,&e);
	UserThreadCreate(print,&f);
	UserThreadCreate(print,&g);
	UserThreadCreate(print,&h);

	//End();
	//Not reached
	return 0;
}