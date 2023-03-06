#include<stdio.h>
int digit(char c)
{
    return c >= '0' && c <= '9' ? c - '0' : (c & 31) + 9;
}
int atoi(const char *str)
{
    int a = 0;
    for(; *str; ++str)
        a = a << 4 | digit(*str);
    return a;
}
int main(int argl, char **argv)
{
    for(int i = 1; i < argl; i++)
    {
        fputc(atoi(argv[i]),stdout);
    }
    fputc('\n',stdout);
	return 0;
}
