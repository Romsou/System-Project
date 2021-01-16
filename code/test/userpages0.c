#include "syscall.h"

void print(void *arg){
	int wich = *(int *)arg;
	for(int i=0; i < 10 ;  i++){
		PutString("On affiche \t");
		PutInt(wich);
		PutChar('\t');
		PutInt(i);
		PutChar('\n');
	}
}

int main(){
	int a=1, b=2, c=3;
	int t1 = UserThreadCreate(print,&a);
	int t2 = UserThreadCreate(print,&b);
	int t3 = UserThreadCreate(print,&c);

	UserThreadJoin(t1);
	UserThreadJoin(t2);
	UserThreadJoin(t3);

	End();
	return 0;


}