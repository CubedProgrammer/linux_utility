#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argl, char *argv[])
{
    char fname[91] = "/proc/cmdline";
    size_t fsz, capa;
    char *cmdline, *tmp;
    FILE *fh;
    int cnt = 0;
    int succ = 1;
    if(argl == 2)
        sprintf(fname, "/proc/%s/cmdline", argv[1]);
    fh = fopen(fname, "r");
    if(fh == NULL)
        fputs("Process may not exist, or some other error occurred.\n", stderr);
    else
    {
        capa = 60;
        fsz = 0;
        cmdline = malloc(capa);
        if(cmdline == NULL)
            perror("malloc failed");
        else
        {
            for(int ch = fgetc(fh); !feof(fh); ch = fgetc(fh))
            {
                if(fsz == capa)
                {
                    capa += capa >> 1;
                    tmp = malloc(capa);
                    if(tmp == NULL)
                        perror("malloc failed");
                    else
                        memcpy(tmp, cmdline, fsz);
                    free(cmdline);
                    cmdline = tmp;
                }
                cmdline[fsz] = ch;
                ++fsz;
            }
            for(const char *it = cmdline; it != cmdline + fsz; ++it, ++cnt)
            {
                printf("${%i}\n", cnt);
                puts(it);
                for(; *it != '\0'; ++it);
            }
            free(cmdline);
            succ = 0;
        }
        fclose(fh);
    }
    return succ;
}
