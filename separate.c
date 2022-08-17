#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    int nullfd = open("/dev/null", O_RDWR);
    int pid = fork();
    if(pid == 0)
    {
        setsid();
        dup2(nullfd, STDIN_FILENO);
        dup2(nullfd, STDOUT_FILENO);
        dup2(nullfd, STDERR_FILENO);
        execvp(argv[1], argv + 1);
        exit(1);
    }
    else
    {
        printf("Process with id %d has been created.\n", pid);
        close(nullfd);
    }
    return 0;
}
