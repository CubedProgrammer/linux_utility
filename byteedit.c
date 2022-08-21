#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>

const char TMPDIR[] = "/tmp/cpbyteedittmpfile";
int maketmps(int fcnt, char *files[])
{
    int succ;
    int fromfd, tofd;
    char path[2601];
    size_t tmplen = sizeof TMPDIR;
    if(access(TMPDIR, F_OK))
        succ = mkdir(TMPDIR, 0755);
    else
        succ = 0;
    if(succ == 0)
    {
        strcpy(path, TMPDIR);
        path[tmplen - 1] = '/';
        for(int i = 0; i < fcnt; ++i)
        {
            strcpy(path + tmplen, files[i]);
            fromfd = open(files[i], O_RDONLY);
            if(fromfd == -1)
            {
                perror("opening file failed");
                continue;
            }
            tofd = open(path, O_WRONLY, O_CREAT);
            if(tofd == -1)
            {
                close(fromfd);
                perror("opening temporary file failed");
                continue;
            }
            close(fromfd);
            close(tofd);
        }
    }
    else
        perror("mkdir failed");
    return succ;
}

int main(int argl, char *argv[])
{
    int succ = 0;
    const char *ed = "vi";
    char *edenv;
    char edenvbuf[91];
    edenv = getenv("EDITOR");
    if(edenv != NULL)
    {
        strcpy(edenvbuf, edenv);
        ed = edenvbuf;
    }
    return succ;
}
