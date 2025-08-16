#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
int run(unsigned target, char *cmd[])
{
    long current = time(NULL);
    long daytime = current % 86400;
    long sleeptime = target - daytime;
    int pid;
    int res = 0;
    if(target < daytime)
        sleeptime += 86400;
    pid = fork();
    if(pid == 0)
    {
        int null;
        sleep(sleeptime);
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
        printf("%s HOUR[:MINUTE[:SECOND]] [MINUTE[:SECOND]] [SECOND] command args...\n", argv[0]);
    else
    {
        unsigned time[3] = {0, 0, 0};
        unsigned used = 0;
        char *str;
        int start = 1;
        for(; start < argl && used < 3; ++start)
        {
            str = argv[start];
            time[used] = strtoul(str, &str, 10);
            if(str == argv[start])
            {
                used = 3;
                --start;
            }
            else
            {
                ++used;
                for(; *str != '\0' && used < 3; ++used)
                    time[used] = strtoul(str + 1, &str, 10);
            }
        }
        succ = run((time[0] * 3600 + time[1] * 60 + time[2]) % 86400, argv + start);
    }
    return succ;
}
