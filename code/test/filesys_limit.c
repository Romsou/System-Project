#include "syscall.h"
//launch
/*
*./nachos-step5 -f; ./nachos-step5 -cp filesys_limit filesys_limit; ./nachos-step5 -x filesys_limit;
*/

char *get(){
    static int COUNT = 0;
    switch(COUNT){
        case 0: COUNT = COUNT +1; return "rep1/f1";
        case 1: COUNT = COUNT +1; return "rep1/f2";
        case 2: COUNT = COUNT +1; return "rep1/f3";
        case 3: COUNT = COUNT +1; return "rep1/f4";
        case 4: COUNT = COUNT +1; return "rep1/f5";
        case 5: COUNT = COUNT +1; return "rep1/f6";
        case 6: COUNT = COUNT +1; return "rep1/f7";
        case 7: COUNT = COUNT +1; return "rep1/f8";
        case 8: COUNT = COUNT +1; return "rep2/f9";
        case 9: COUNT = COUNT +1; return "rep2/f10";
        case 10: COUNT = COUNT +1; return "rep2/f11";
        case 11: COUNT = COUNT +1; return "rep2/f12";
        case 12: COUNT = COUNT +1; return "rep2/f13";
        case 13: COUNT = COUNT +1; return "rep2/f14";
        case 14: COUNT=0; return "rep2/f15";
        default: break;
    }
    return "";
}

void CreateFiles(){
    Create("rep1/f1");
    Create("rep1/f2");
    Create("rep1/f3");
    Create("rep1/f4");
    Create("rep1/f5");
    Create("rep1/f6");
    Create("rep1/f7");
    Create("rep1/f8");
    Create("rep2/f9");
    Create("rep2/f10");
    Create("rep2/f11");
    Create("rep2/f12");
    Create("rep2/f13");
    Create("rep2/f14");
    Create("rep2/f15");
}

int main ()
{
    Create("./rep1/");
    Create("./rep2/");

    CreateFiles();

    OpenFileId tabIds[50];

    for(int i = 0; i < 50; i++){
        PutInt(i);
        PutChar(':');
        tabIds[i] = Open(get());
        PutInt(tabIds[i]);
        PutChar(',');
        //Close(tabIds[i]);
    }
    PutChar('\n');

  
    End();
    /* not reached */
    return 0;
}