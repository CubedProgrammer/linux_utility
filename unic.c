#include<stdio.h>
int main(int argl, char *argv[])
{
    char *arg;
    for(int i = 1; i < argl; ++i)
    {
        arg = argv[i];
        for(const char *it = arg; *it != 0; ++it)
            printf("%02x", (unsigned)*it & 0xff);
        putchar('\n');
    }
    return argl == 1;
}
