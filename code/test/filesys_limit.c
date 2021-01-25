#include "syscall.h"
//launch
/*
*./nachos-step5 -f; ./nachos-step5 -cp filesys_llimit filesys_limit;
		./nachos-step5 -x filesys_limit; ./nachos-step5 -D
*/

char *get(){
    static int COUNT = 0;
    switch(COUNT){
        case 0: COUNT = COUNT +1; return "f1";
        case 1: COUNT = COUNT +1; return "f2";
        case 2: COUNT = COUNT +1; return "f3";
        case 3: COUNT = COUNT +1; return "f4";
        case 4: COUNT = COUNT +1; return "f5";
        case 5: COUNT = COUNT +1; return "f6";
        case 6: COUNT = COUNT +1; return "f7";
        case 7: COUNT = COUNT +1; return "f8";
        case 8: COUNT = COUNT +1; return "f9";
        case 9: COUNT = COUNT +1; return "f10";
        case 10: COUNT = COUNT +1; return "f11";
        case 11: COUNT = COUNT +1; return "f12";
        case 12: COUNT = COUNT +1; return "f13";
        case 13: COUNT = COUNT +1; return "f14";
        case 14: COUNT=0; return "f15";
        default: return "";
    }
    return "";
}

void CreateFithteenFiles(){
    Create("f1");
    Create("f2");
    Create("f3");
    Create("f4");
    Create("f5");
    Create("f6");
    Create("f7");
    Create("f8");
    Create("f9");
    Create("f10");
    Create("f11");
    Create("f12");
    Create("f13");
    Create("f14");
    Create("f15");
}

int main ()
{
    CreateFithteenFiles();

    OpenFileId tabIds[50];

    for(int i = 0; i < 50; i++){
        PutInt(i);
        PutChar(',');
        tabIds[i] = Open(get());
        Close(tabIds[i]);
    }

  
    End();
    /* not reached */
    return 0;
}