#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
int main(int argl, char *argv[])
{
    char fname[91] = "/proc/cmdline";
    size_t fsz;
    char *cmdline;
    FILE *fh;
    struct stat fdat;
    int cnt = 0;
    int succ = 1;
    if(argl == 2)
        sprintf(fname, "/proc/%s/cmdline", argv[1]);
    fh = fopen(fname, "r");
    if(fh == NULL)
        fputs("Process may not exist, or some other error occurred.\n", stderr);
    else
    {
        stat(fname, &fdat);
        fsz = fdat.st_size;
        cmdline = malloc(fsz);
        if(cmdline == NULL)
            perror("malloc failed");
        else
        {
            fread(cmdline, 1, fsz, fh);
            printf("%zu\n", fsz);
            for(const char *it = cmdline; it != cmdline + fsz; ++it, ++cnt)
            {
                printf("${%i}\n", cnt);
                puts(it);
                for(; *it != '\0'; ++it);
            }
            succ = 0;
        }
        fclose(fh);
    }
    return succ;
}
