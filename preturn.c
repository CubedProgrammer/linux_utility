#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
int runcmd(const char *cmd, char *args[])
{
    int status = -1;
    int pid = fork();
    if(pid > 0)
    {
        if(waitpid(pid, &status, 0) == -1)
            perror("waitpid failed");
    }
    else if(pid < 0)
        perror("fork failed");
    else
    {
        execvp(cmd, args);
        perror("execvp failed");
        exit(1);
    }
    return status;
}
int main(int argl, char *argv[])
{
    int succ = 1;
    if(argl < 3)
        fputs("The first argument is the radix, b for two, d for ten, and x for sixteen. Then comes the program arguments.\n", stderr);
    else
    {
        char display[10];
        int radix = 2, dig;
        succ = 0;
        switch(argv[1][0])
        {
            case'b':
                radix = 2;
                break;
            case'd':
                radix = 10;
                break;
            case'x':
                radix = 16;
                break;
            default:
                succ = 1;
                fputs("Unrecognized option for radix.\n", stderr);
        }
        if(succ == 0)
        {
            int status = runcmd(argv[2], argv + 2);
            unsigned ind = 0;
            if(status == -1)
                succ = 1;
            else
            {
                status = WEXITSTATUS(status);
                for(; status > 0; status /= radix)
                {
                    dig = status % radix;
                    if(dig > 9)
                        display[ind] = dig + 'A' - 10;
                    else
                        display[ind] = dig + '0';
                    ++ind;
                }
                for(unsigned i = 0; i < ind >> 1; ++i)
                {
                    radix = display[i];
                    display[i] = display[ind - i - 1];
                    display[ind - i - 1] = radix;
                }
                if(ind == 0)
                    display[ind++] = '0';
                display[ind] = '\0';
                puts(display);
            }
        }
    }
    return succ;
}
