#include<stdio.h>
#include<string.h>
int main(int argl, char *argv[])
{
    char *arg;
    size_t len, half;
    for(int i = 1; i < argl; ++i)
    {
        arg = argv[i];
        len = strlen(arg);
        half = len >> 1;
        for(size_t j = 0; j < half; ++j)
            arg[j] ^= arg[len - j - 1] ^= arg[j] ^= arg[len - j - 1];
        puts(arg);
    }
    return argl == 1;
}
