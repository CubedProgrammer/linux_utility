#include<stdio.h>
#define OPENING "<p>"
#define CLOSING "</p>"
void convert(FILE *restrict in, FILE *restrict out)
{
    char lastlf = 1;
    for(int c = fgetc(in); c != EOF; c = fgetc(in))
    {
        if(lastlf)
        {
            fputs(OPENING, out);
            lastlf = 0;
        }
        if(c == '\n')
        {
            lastlf = 1;
            fputs(CLOSING, out);
        }
        switch(c)
        {
            case'>':
                fputs("&gt;", out);
                break;
            case'<':
                fputs("&lt;", out);
                break;
            case'&':
                fputs("&amp;", out);
                break;
            default:
                fputc(c, out);
                break;
        }
    }
}
int main(int argl, char *argv[])
{
    FILE *i = stdin;
    FILE *o = stdout;
    if(argl > 1)
    {
        i = fopen(argv[1], "r");
        if(i == NULL)
        {
            fprintf(stderr, "Opening %s", argv[1]);
            perror(" failed");
            i = stdin;
        }
        if(argl > 2)
        {
            o = fopen(argv[2], "w");
            if(o == NULL)
            {
                fprintf(stderr, "Opening %s", argv[2]);
                perror(" failed");
                o = stdout;
            }
        }
    }
    convert(i, o);
    if(o != stdout)
        fclose(o);
    if(i != stdin)
        fclose(i);
    return 0;
}
