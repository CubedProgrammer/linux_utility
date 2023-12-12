#include<stdio.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    printf("%d\n", getpid());
    return execvp(argv[1], argv + 1);
}
