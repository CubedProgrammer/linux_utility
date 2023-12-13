#include<stdio.h>
int main(int argl, char *argv[])
{
    if(argl < 4)
        printf("%s <count> f<format> file...\n", argv[0]);
    else
    {
        unsigned cnt = 12;
        FILE *fh;
        char defaultfmt[] = "%u";
        char *fmt = defaultfmt;
        sscanf(argv[1], defaultfmt, &cnt);
        if(argv[2][0] == 'f')
        {
            if(argv[2][1] != '\0')
                fmt = argv[2] + 1;
        }
        else
            fmt = argv[2];
        for(int i = 3; i < argl; ++i)
        {
            fh = fopen(argv[i], "a");
            for(unsigned j = 0; j < cnt; fprintf(fh, fmt, ++j), fputc('\n', fh));
            fclose(fh);
        }
    }
    return 0;
}
