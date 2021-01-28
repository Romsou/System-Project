
#include "syscall.h"

void print(void *j){
	int i = *(int*)j;
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

	int a=1;
	int b=2;
	int c=3;
	int d=4;
	int e=5; 
	int f=6;
	int g=7;
	int h=8;
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
	UserThreadCreate(print,&c);
	UserThreadCreate(print,&d);
	UserThreadCreate(print,&e);	
	UserThreadCreate(print,&f);
	UserThreadCreate(print,&g);
	UserThreadCreate(print,&h);

	End();
}
