#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void indentfile(FILE *in, FILE *out, int cnt, unsigned start, unsigned end)
{
    char arr[16384], indented[64];
    unsigned arrlen, lnpos;
    unsigned spaces = 0, ln = 0;
    int ch;
    char rtext = 0;
    ++end;
    if(cnt > 0)
        memset(indented, ' ', cnt);
    while(!feof(in))
    {
        arrlen = fread(arr, 1, sizeof arr, in);
        lnpos = 0;
        if(ln >= end)
            fwrite(arr, 1, arrlen, out);
        else
        {
            for(unsigned i = 0; i < arrlen; ++i)
            {
                if(arr[i] == '\n')
                {
                    ++ln;
                    if(ln >= start && ln < end)
                    {
                        if(cnt < 0)
                            lnpos -= cnt;
                        else
                            fwrite(indented, 1, cnt, out);
                    }
                    fwrite(arr + lnpos, 1, i + 1 - lnpos, out);
                    lnpos = i + 1;
                }
            }
        }
    }
}
int main(int argl, char *argv[])
{
    if(argl == 1)
        printf("Usage: %s <number>[,start,end] files...\n", argv[0]);
    else
    {
        int cnt, start, end;
        int arr[3] = {0, 1, 1048576};
        unsigned ind = 0;
        char *numopt = argv[1], *last = numopt;
        FILE *i = stdin, *o = stdout;
        for(char *it = numopt; *it != '\0'; ++it)
        {
            if(*it == ',')
            {
                *it = '\0';
                arr[ind] = atoi(last);
                ++ind;
                last = it + 1;
            }
        }
        arr[ind] = atoi(last);
        cnt = arr[0];
        start = arr[1], end = arr[2];
        if(argl > 2)
        {
            i = fopen(argv[2], "r");
            if(argl > 3)
                o = fopen(argv[3], "w");
        }
        indentfile(i, o, cnt, start, end);
    }
    return 0;
}
