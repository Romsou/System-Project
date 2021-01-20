#include "syscall.h"


#define THIS "abcdefghijklmnopqrstuvwxyz\n"

const int N = 10;

void puts(void *s)
{
	char *p;
	for(p=(char*)s ; *p != '\0' ; p++){
		PutChar(*p);
	}
}

int main(){
	PutString("TEST1\n");
	UserThreadCreate(puts,THIS);
	End();
	return 0;
}