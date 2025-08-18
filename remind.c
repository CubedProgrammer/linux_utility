#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
int run(unsigned target, char *cmd[])
{
    int res = 0;
    int pid = fork();
    if(pid == 0)
    {
        int null;
        sleep(target);
        null = open("/dev/null", O_RDWR);
        dup2(null, STDIN_FILENO);
        dup2(null, STDOUT_FILENO);
        dup2(null, STDERR_FILENO);
        close(null);
        setsid();
        res = execvp(*cmd, cmd);
    }
    else if(pid < 0)
        perror("fork failed");
    return res;
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl == 1)
        printf("%s [WEEK] [DAY] [HOUR] [MINUTE] SECOND command args...\n", *argv);
    else
    {
        const unsigned long scalars[] = {1, 60, 3600, 86400, 604800};
        int start = 1;
        char empty[] = "";
        char *str = empty;
        unsigned long time[5];
        unsigned used = 0;
        unsigned long total = 0;
        for(; start < argl && used < 5 && *str == '\0'; ++start)
        {
            time[used] = strtoul(argv[start], &str, 10);
            ++used;
        }
        if(*str)
        {
            --start;
            --used;
        }
        for(unsigned i = used; i > 0; --i)
            total += time[i - 1] * scalars[used - i];
        succ = run(total, argv + start);
    }
    return succ;
}
