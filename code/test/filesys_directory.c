#include "syscall.h"

/*
*./nachos-step5 -f; ./nachos-step5 -cp filesys_directory filesys_directory; ./nachos-step5 -x filesys_directory;
*/

int main(){
	Create("./rep1/");
    Create("./rep2/");
    Remove("./rep1/");
    Open("./rep2/");
    Create("toto");
    End();
	return 0;
} 