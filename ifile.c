#include<stdio.h>
int main(int argl, char *argv[])
{
    char unsigned buffer[8192];
    FILE *f = NULL;
    if(argl > 1)
        f = fopen(argv[1], "r");
    if(f == NULL)
        f = stdin;
    while(!feof(f))
        fwrite(buffer, 1, fread(buffer, 1, sizeof buffer, f), stdout);
    if(f != stdin)
        fclose(f);
    return 0;
}
