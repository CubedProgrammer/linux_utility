#include<stdio.h>
int main(int argl, char *argv[])
{
    if(argl > 1)
    {
        char *arg;
        for(int i = 1; i < argl; ++i)
        {
            arg = argv[i];
            for(const char *it = arg; *it != 0; ++it)
                printf("%02x", *it);
            putchar('\n');
        }
    }
    return argl == 1;
}
