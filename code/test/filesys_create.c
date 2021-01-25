#include "syscall.h"

int main ()
{
    Create("f1");
    int fd = Open("f1");
    char *s = "Blablabla";
    char *res = "";
    int len = 10;

    Write(s,len,fd);
    Read(res,len,fd);
    PutString(res);
    Close(fd);

    End();
    /* not reached */
    return 0;
}