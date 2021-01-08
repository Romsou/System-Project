#include "system.h"

void print(int i){
	PutInt(i);
	if(i%2)
		PutString("Je suis un nombre impair\n");
	else
		PutString("Je suis un nombre pair\n");
}

int main(){
	PutString("Debut du main...\n");
	UserThreadCreate(print,12);
	UserThreadCreate(print,13);
}