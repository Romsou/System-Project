
#include "syscall.h"

void print(void *j){
	int i = *(int *)j;
	PutInt(i);
	if(i%2)
		PutString("Je suis un nombre impair\n");
	else
		PutString("Je suis un nombre pair\n");
}

int main(){
	PutString("Debut du main...\n");
	int a = 12, b=13;
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
}