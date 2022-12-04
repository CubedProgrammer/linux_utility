#include<stdio.h>
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
        int ret;
        FILE *fh;
        for(int i = 1; i < argl; ++i)
        {
            ret = stat(argv[i], &fdat) * -1;
            if(ret)
            {
                fprintf(stderr, "stat %s", argv[i]);
                perror(" failed");
            }
            else
            {
                fh = fopen(argv[i], "wb");
                if(fh == NULL)
                {
                    fprintf(stderr, "%s", argv[i]);
                    perror(" could not be opened");
                }
                else
                {
                    fsz = fdat.st_size;
                    cnt = 0;
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
