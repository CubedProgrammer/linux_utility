#include<stdio.h>
int main(int argl, char *argv[])
{
    char unsigned buffer[8192];
    FILE *f = NULL;
    if(argl > 1)
        f = fopen(argv[1], "w");
    if(f == NULL)
        f = stdout;
    while(!feof(stdin))
        fwrite(buffer, 1, fread(buffer, 1, sizeof buffer, stdin), f);
    return 0;
}
