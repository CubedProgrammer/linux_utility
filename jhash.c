#include<stdio.h>
int jhash(const char *it)
{
    unsigned h = 0;
    for(; *it != '\0'; ++it)
        h = h * 31 + *it;
    return h;
}
int main(int argl, char *argv[])
{
    for(int i = 1; i < argl; ++i)
        printf("%d\n", jhash(argv[i]));
    return 0;
}
