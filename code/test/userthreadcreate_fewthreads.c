/*
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
	*/
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

//TODO assez gro problème ici, au niveau du passage des arguments on de leur lecture, peut etre voir du coté de la pile dans StartUserThread
//Problème sur le passage des int, le passage des chaines semble OK (voir userpages0 et userpages1)

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

	int ret;
	/*UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
	UserThreadCreate(print,&c);
	UserThreadCreate(print,&d);
	UserThreadCreate(print,&e);	
	UserThreadCreate(print,&f);
	UserThreadCreate(print,&g);
	UserThreadCreate(print,&h);*/

	ret = UserThreadCreate(print,&a);
	UserThreadJoin(ret);

	ret = UserThreadCreate(print,&b);
	UserThreadJoin(ret);

	ret = UserThreadCreate(print,&c);
	UserThreadJoin(ret);

	ret = UserThreadCreate(print,&d);
	UserThreadJoin(ret);

	ret = UserThreadCreate(print,&e);
	UserThreadJoin(ret);
	
	ret = UserThreadCreate(print,&f);
	UserThreadJoin(ret);

	ret = UserThreadCreate(print,&g);
	UserThreadJoin(ret);

	ret = UserThreadCreate(print,&h);
	UserThreadJoin(ret);

	//End();
	//Not reached
	return 0;
}