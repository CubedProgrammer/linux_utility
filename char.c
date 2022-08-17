#include<stdio.h>
#include<stdlib.h>
int main(int argl, char **argv)
{
    for(int i = 1; i < argl; i++)
    {
        fputc(atoi(argv[i]),stdout);
    }
    fputc('\n',stdout);
	return 0;
}
