#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
char zero[8192];
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl == 1)
        puts("Name files to destroy.");
    else
    {
        struct stat fdat;
        size_t fsz, cnt;
        char*filename;
        int ret;
        FILE *fh;
        for(int i = 1; i < argl; ++i)
        {
            fsz = 0;
            filename = argv[i];
            if(argv[i][0] == '-')
                fsz = strtoul(argv[i] + 1, &filename, 10);
            ret = stat(filename, &fdat) * -1;
            if(ret)
            {
                fprintf(stderr, "stat %s", argv[i]);
                perror(" failed");
            }
            else
            {
                fh = fopen(filename, "wb");
                if(fh == NULL)
                {
                    fprintf(stderr, "%s", argv[i]);
                    perror(" could not be opened");
                }
                else
                {
                    if(fsz == 0)
                        fsz = fdat.st_size;
                    cnt = 0;
                    fwrite(zero, 1, fsz & sizeof(zero) - 1, fh);
                    fsz &= -sizeof(zero);
                    while(cnt < fsz)
                        cnt += fwrite(zero, 1, sizeof(zero), fh);
                    fclose(fh);
                }
            }
            succ += ret;
        }
    }
    return succ;
}
