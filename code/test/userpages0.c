#include "syscall.h"

void print(void *arg){
	int wich = *(int *)arg;
	for(int i=0; i < 5 ;  i++){
		PutString("On affiche \t");
		PutInt(wich);
		PutChar('\t');
		PutInt(i);
		PutChar('\n');
	}
}


int main(){
	PutString("TEST0\n");
	int a=1, b=2, c=3;
	UserThreadCreate(print,&a);
	UserThreadCreate(print,&b);
	UserThreadCreate(print,&c);

	End();
	return 0;
}