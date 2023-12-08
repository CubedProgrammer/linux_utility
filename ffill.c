#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void fill(FILE *fh, char c, size_t size)
{
    char cbuf[8192];
    memset(cbuf, c, sizeof cbuf);
    size_t quotient = size >> 13, remainder = size & 8191;
    for(size_t i = 0; i < quotient; ++i)
        fwrite(cbuf, 1, sizeof(cbuf), fh);
    fwrite(cbuf, 1, remainder, fh);
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl < 3)
        printf("Usage: %s <char> <size> [FILE...]\n", argv[0]);
    else if(argl == 3)
        fill(stdin, strtol(argv[1], NULL, 16), strtoul(argv[2], NULL, 10));
    else
    {
        FILE *fh;
        char ch = strtol(argv[1], NULL, 16);
        size_t cnt = strtoul(argv[2], NULL, 10);
        for(int i = 3; i < argl; ++i)
        {
            fh = fopen(argv[i], "w");
            if(fh == NULL)
            {
                fprintf(stderr, "Opening %s", argv[i]);
                perror(" failed");
                succ += succ < 126;
            }
            else
            {
                fill(fh, ch, cnt);
                fclose(fh);
            }
        }
    }
    return succ;
}
