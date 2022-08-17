#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    if(argl > 1)
    {
        char oflag[] = "-o";
        char obj[2048], exe[2048];
        strcpy(obj, argv[1]);
        strcat(obj, ".o");
        strcpy(exe, obj);
        strcat(exe, "ut");
        char compiler[128];
        strcpy(compiler, getenv("COMPILERPP"));
        char **pargls = malloc((argl + 3) * sizeof(*pargls));
        pargls[0] = compiler;
        pargls[1] = obj;
        pargls[2] = oflag;
        pargls[3] = exe;
        memcpy(pargls + 4, argv + 2, (argl - 1) * sizeof(*argv));
        execvp(compiler, pargls);
    }
    else
        fputs("Input the object file without the .o.\n", stderr);
    return 1;
}
