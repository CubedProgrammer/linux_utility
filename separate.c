#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    int ec, succ = 0;
    int verbosity = 0;
    int p[2];
    int nullfd = open("/dev/null", O_RDWR);
    pipe(p);
    int flag = fcntl(p[1], F_GETFD);
    flag |= FD_CLOEXEC;
    fcntl(p[1], F_SETFD, flag);
    int pid = fork();
    if(pid == 0)
    {
        close(p[0]);
        setsid();
        dup2(nullfd, STDIN_FILENO);
        dup2(nullfd, STDOUT_FILENO);
        dup2(nullfd, STDERR_FILENO);
        execvp(argv[1], argv + 1);
        succ = 1;
        ec = errno;
        write(p[1], &ec, sizeof ec);
        close(p[1]);
    }
    else if(pid == -1)
    {
        perror("fork failed");
        succ = 1;
    }
    else
    {
        close(p[1]);
        close(nullfd);
        long cnt = read(p[0], &ec, sizeof ec);
        close(p[0]);
        if(cnt > 0)
        {
            errno = ec;
            fprintf(stderr, "Executing program %s", argv[1]);
            perror(" failed");
            succ = 1;
        }
        else
        {
            char *verbosityvar = getenv("LU_SEPARATE_VERBOSE");
            if(verbosityvar != NULL)
                verbosity = atoi(verbosityvar);
            if(verbosity == 2)
                printf("Process with id %d has been created.\n", pid);
            else if(verbosity == 1)
                printf("%d\n", pid);
        }
    }
    return succ;
}
