#include<stdio.h>
void printnums(FILE *fh, unsigned cnt, const char *fmt)
{
    for(unsigned j = 0; j < cnt; fprintf(fh, fmt, ++j), fputc('\n', fh));
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl < 3)
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
        if(argl == 3)
            printnums(stdout, cnt, fmt);
        for(int i = 3; i < argl; ++i)
        {
            fh = fopen(argv[i], "a");
            if(fh == NULL)
            {
                fprintf(stderr, "Opening %s", argv[i]);
                perror(" failed");
                succ = 1;
            }
            else
            {
                printnums(fh, cnt, fmt);
                fclose(fh);
            }
        }
    }
    return succ;
}
