#define _GNU_SOURCE
#include<fcntl.h>
#include<stdio.h>
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl != 3)
        puts("Name two files, their names and locations will be swapped.");
    else
    {
        succ = renameat2(AT_FDCWD, argv[1], AT_FDCWD, argv[2], RENAME_EXCHANGE) * -1;
        if(succ)
            perror("renameat2 failed");
    }
    return succ;
}
