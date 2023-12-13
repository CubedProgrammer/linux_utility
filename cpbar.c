#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/sendfile.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
int copyfile(const char *src, const char *dest, unsigned step, unsigned barlen)
{
    int succ = -1;
    struct stat fdat;
    if(stat(src, &fdat))
    {
        fprintf(stderr, "stat %s", src);
        perror(" failed");
    }
    else
    {
        size_t fsize = fdat.st_size, tot = 0;
        int mode = fdat.st_mode;
        if(S_ISREG(mode))
        {
            ssize_t cnt;
            char bar[961];
            unsigned progress;
            int in = open(src, O_RDONLY);
            int out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, mode);
            if(in == -1 || out == -1)
            {
                fprintf(stderr, "Copying %s to %s", src, dest);
                perror(" failed");
            }
            else if(fsize > step)
            {
                memset(bar, '-', barlen);
                bar[barlen] = '\0';
                succ = 0;
                while(tot < fsize)
                {
                    cnt = sendfile(out, in, NULL, step);
                    if(cnt == -1)
                    {
                        fprintf(stderr, "sendfile %s to %s", src, dest);
                        perror(" failed");
                        tot = fsize;
                        cnt = 0;
                        succ = -1;
                    }
                    tot += cnt;
                    progress = tot * barlen / fsize;
                    memset(bar, '#', progress);
                    printf("%zu/%zu [%s]\r", tot, fsize, bar);
                }
                putchar('\n');
            }
            else if(sendfile(out, in, NULL, fsize) == -1)
            {
                fprintf(stderr, "sendfile %s to %s", src, dest);
                perror(" failed");
            }
            else
                succ = 0;
        }
        else
            fprintf(stderr, "%s is not a regular file.\n", src);
    }
    return succ;
}
int copyto(char *namearr[], const char *dir, unsigned arrlen, unsigned step, unsigned barlen)
{
    const char *srcpath, *srcname;
    int succ = 0;
    char *dest = NULL;
    unsigned destlen = 0, srcnamelen;
    unsigned dirlen = strlen(dir);
    for(unsigned i = 0; i < arrlen; ++i)
    {
        srcpath = namearr[i];
        srcname = strrchr(srcpath, '/');
        if(srcname == NULL)
            srcname = srcpath;
        else
            ++srcname;
        srcnamelen = strlen(srcname);
        if(destlen < srcnamelen + dirlen + 1)
        {
            if(dest != NULL)
                free(dest);
            destlen = dirlen + srcnamelen + 1;
            dest = malloc(destlen + 1);
        }
        memcpy(dest, dir, dirlen);
        dest[dirlen] = '/';
        memcpy(dest + dirlen + 1, srcname, srcnamelen + 1);
        succ += copyfile(srcpath, dest, step, barlen);
    }
    if(dest != NULL)
        free(dest);
    return succ;
}
int main(int argl, char *argv[])
{
    if(argl < 3)
    {
        fprintf(stderr, "%s <src> <dest>\n", argv[0]);
        return 1;
    }
    else
    {
        struct winsize dim;
        unsigned col = 80;
        unsigned step = 2097152;
        char *arg;
        int optend = argl;
        int nxtarg = 0;
        if(ioctl(STDIN_FILENO, TIOCGWINSZ, &dim) == 0)
            col = dim.ws_col;
        for(int i = 1; i < optend; ++i)
        {
            arg = argv[i];
            if(nxtarg)
            {
                switch(nxtarg)
                {
                    case 1:
                        step = atoi(arg);
                }
                nxtarg = 0;
            }
            else if(arg[0] == '-')
            {
                switch(arg[1])
                {
                    case'-':
                        if(strcmp(arg + 2, "barthresh") == 0)
                            nxtarg = 1;
                }
            }
            else
                optend = i;
        }
        argv += optend - 1;
        argl -= optend - 1;
        return argl == 3 ? copyfile(argv[1], argv[2], step, col - 40) : copyto(argv + 1, argv[argl - 1], argl - 2, step, col - 40);
    }
}
