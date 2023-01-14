#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<time.h>
#include<unistd.h>

void savefiles(int fcnt, char *files[])
{
    int fromfd, tofd;
    char path[2601];
    unsigned char inbuf[16384];
    char outbuf[16384];
    ssize_t bc;
    unsigned outpos, num;
    struct stat fdat;
    char ch, full = 0;
    for(int i = 0; i < fcnt; ++i)
    {
        strcpy(path, files[i]);
        sprintf(path + strlen(path), "%08x", (int) time(NULL));
        if(stat(files[i], &fdat) == -1)
        {
            perror("stat failed");
            continue;
        }
        fromfd = open(files[i], O_RDONLY);
        if(fromfd == -1)
        {
            perror("opening file failed");
            continue;
        }
        tofd = open(path, O_WRONLY | O_CREAT, fdat.st_mode);
        if(tofd == -1)
        {
            close(fromfd);
            perror("opening temporary file failed");
            continue;
        }
        outpos = 0;
        bc = sizeof inbuf;
        while(bc == sizeof(inbuf))
        {
            bc = read(fromfd, inbuf, sizeof inbuf);
            if(bc == -1)
                perror("reading failed");
            else
            {
                for(unsigned i = 0; i < bc; ++i)
                {
                    ch = inbuf[i];
                    if(ch >= '0' && ch <= '9')
                    {
                        num = num << 4 | ch - '0';
                        ++full;
                    }
                    if(ch >= 'a' && ch <= 'f')
                        ch -= 32;
                    if(ch >= 'A' && ch <= 'F')
                    {
                        num = num << 4 | ch - 'A' + 10;
                        ++full;
                    }
                    if(full == 2)
                    {
                        outbuf[outpos++] = num;
                        full = 0;
                    }
                    if(outpos == 16384)
                    {
                        write(tofd, outbuf, outpos);
                        outpos = 0;
                    }
                }
            }
        }
        if(outpos)
            write(tofd, outbuf, outpos);
        close(fromfd);
        close(tofd);
        if(rename(path, files[i]) == -1)
            perror("rename failed");
    }
}

int maketmps(int fcnt, char *files[])
{
    int succ;
    int fromfd, tofd;
    char path[2601];
    unsigned char inbuf[16384];
    char outbuf[16384];
    ssize_t bc;
    unsigned rc = 24, cc = 80;
    unsigned outpos;
    struct winsize tsz;
    struct stat fdat;
    succ = ioctl(STDIN_FILENO, TIOCGWINSZ, &tsz);
    if(succ == 0)
    {
        rc = tsz.ws_row;
        cc = tsz.ws_col;
        for(int i = 0; i < fcnt; ++i)
        {
            strcpy(path, files[i]);
            sprintf(path + strlen(path), "%08x", (int) time(NULL));
            if(stat(files[i], &fdat) == -1)
            {
                perror("stat failed");
                continue;
            }
            fromfd = open(files[i], O_RDONLY);
            if(fromfd == -1)
            {
                perror("opening file failed");
                continue;
            }
            tofd = open(path, O_WRONLY | O_CREAT, fdat.st_mode);
            if(tofd == -1)
            {
                close(fromfd);
                perror("opening temporary file failed");
                continue;
            }
            cc -= cc % 3;
            bc = sizeof inbuf;
            outpos = 0;
            while(bc == sizeof(inbuf))
            {
                bc = read(fromfd, inbuf, sizeof inbuf);
                if(bc == -1)
                    perror("reading failed");
                else
                {
                    for(unsigned i = 0; i < bc; ++i)
                    {
                        sprintf(outbuf + outpos, "%02X", inbuf[i]);
                        outbuf[outpos + 2] = outpos % (cc - 9) == 0 ? '\n' : ' ';
                        outpos += 3;
                        if(outpos == 16383)
                        {
                            write(tofd, outbuf, outpos);
                            outpos = 0;
                        }
                    }
                }
            }
            if(outpos)
                write(tofd, outbuf, outpos);
            close(fromfd);
            close(tofd);
            if(rename(path, files[i]) == -1)
                perror("rename failed");
        }
    }
    else
        perror("ioctl TIOCGWINSZ failed");
    return succ;
}

int execed(const char *ed, char *argv[])
{
    int status;
    int pid = fork();
    if(pid > 0)
    {
        int succ = waitpid(pid, &status, 0);
        if(succ == -1)
        {
            perror("waitpid failed");
            status = -1;
        }
    }
    else if(pid < 0)
        perror("fork failed");
    else
    {
        execvp(ed, argv);
        exit(1);
    }
    return WEXITSTATUS(status);
}

int main(int argl, char *argv[])
{
    int succ = 0;
    char viarr[] = "vi";
    char *ed = viarr;
    char *edenv;
    char edenvbuf[91];
    edenv = getenv("EDITOR");
    if(edenv != NULL)
    {
        strcpy(edenvbuf, edenv);
        ed = edenvbuf;
    }
    *argv = ed;
    succ = maketmps(argl - 1, argv + 1);
    if(succ == 0)
    {
        succ = execed(ed, argv);
        if(succ != 0)
            printf("%s exited with status %d.\n", ed, succ);
        savefiles(argl - 1, argv + 1);
    }
    return succ;
}
