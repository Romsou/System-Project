#include "syscall.h"

#define THIS "aaa"
// #define THAT "bbb"

// const int N = 10;

void puts(void *s)
{
	char *p;
	for(p=(char*)s ; *p != '\0' ; p++){
		PutChar(*p);
	}
}

/*void f(void *s){
	int i;
	for(i=0 ; i<N; i++){
		puts(s);
	}
}*/

int main(){
	UserThreadCreate(puts,THIS);
	// f((void *)THAT);
	return 0;
}