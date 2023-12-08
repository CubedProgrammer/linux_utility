#include<fcntl.h>
#include<stdio.h>
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
        if(ioctl(STDIN_FILENO, TIOCGWINSZ, &dim) == 0)
            col = dim.ws_col;
        return copyfile(argv[1], argv[2], 2097152, col - 40);
    }
}
