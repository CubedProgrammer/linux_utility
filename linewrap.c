#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void lnwrap(unsigned cnt, FILE *in, FILE *out)
{
    unsigned lnstart, bytes;
    unsigned leftover = 0;
    char firstspace = 0;
    char *buf = malloc(cnt);
    while(!feof(in))
    {
        bytes = fread(buf + leftover, 1, cnt - leftover, in) + leftover;
        for(char *it = buf; firstspace; ++it)
        {
            if(*it == ' ' || *it == '\n')
            {
                *it = '\n';
                firstspace = 0;
            }
        }
        for(lnstart = bytes - 1; lnstart > 0 && buf[lnstart] != '\n'; --lnstart);
        if(buf[lnstart] == '\n')
            ++lnstart;
        if(lnstart == 0)
        {
            for(lnstart = bytes - 1; lnstart > 0 && buf[lnstart] != ' '; --lnstart);
            if(lnstart == 0)
            {
                firstspace = 1;
                lnstart = bytes;
            }
            else
                buf[lnstart++] = '\n';
        }
        fwrite(buf, 1, lnstart, out);
        leftover = bytes - lnstart;
        memmove(buf, buf + lnstart, leftover);
    }
    free(buf);
}
int main(int argl, char *argv[])
{
    FILE *in = stdin, *out = stdout;
    int cnt = argv[1] == NULL ? 64 : atoi(argv[1]);
    if(argl > 2)
    {
        in = fopen(argv[2], "r");
        if(argl > 3)
        {
            out = fopen(argv[3], "r+");
            if(out == NULL)
                out = fopen(argv[3], "w");
        }
    }
    in = in == NULL ? stdin : in;
    out = out == NULL ? stdout : out;
    lnwrap(cnt, in, out);
    return 0;
}
