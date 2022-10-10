#include<stdio.h>
int main(int argl, char *argv[])
{
    for(int i = 0; i < argl; ++i)
    {
        printf("${%i}\n", i);
        puts(argv[i]);
    }
    return 0;
}
