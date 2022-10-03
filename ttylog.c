#include<pty.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/select.h>
#include<sys/wait.h>
#include<termios.h>
#include<time.h>
#include<unistd.h>

int slaveterm;

void handle_sigwinch(int x)
{
    if(slaveterm)
    {
        struct winsize dim;
        int succ = ioctl(STDIN_FILENO, TIOCGWINSZ, &dim);
        if(succ == 0)
        {
            succ = ioctl(slaveterm, TIOCSWINSZ, &dim);
            if(succ < 0)
                perror("ioctl TIOCSWINSZ failed");
        }
        else
            perror("ioctl TIOCGWINSZ failed");
    }
}

int pass_data(int pid, int master, FILE *fh)
{
    int succ = 0;
    int ready, status, npid = 0;
    ssize_t bc;
    fd_set fds, *fdsp = &fds;
    struct timeval tv, *tvp = &tv;
    char buf[2048];
    while(npid == 0)
    {
        tv.tv_sec = 120;
        tv.tv_usec = 0;
        FD_ZERO(fdsp);
        FD_SET(STDIN_FILENO, fdsp);
        FD_SET(master, fdsp);
        ready = select(master + 1, fdsp, NULL, NULL, tvp);
        if(ready == -1)
            perror("select failed");
        else if(ready)
        {
            if(FD_ISSET(STDIN_FILENO, fdsp))
            {
                bc = read(STDIN_FILENO, buf, sizeof buf);
                if(bc < 0)
                    perror("read failed");
                else
                    write(master, buf, bc);
            }
            if(FD_ISSET(master, fdsp))
            {
                bc = read(master, buf, sizeof buf);
                if(bc < 0)
                    perror("read master failed");
                else
                {
                    write(STDOUT_FILENO, buf, bc);
                    fwrite(buf, 1, bc, fh);
                }
            }
        }
        npid = waitpid(pid, &status, WNOHANG);
    }
    if(npid == -2)
    {
        succ = npid;
        perror("waitpid failed");
    }
    fclose(fh);
    return succ;
}

int main(int argl, char *argv[])
{
    int succ = 0;
    int pid;
    int master, slave;
    struct termios oldtm, currtm;
    struct winsize dim;
    const char *ofname = "ttylog.log";
    const char *shell = "/usr/bin/bash";
    char *shellenv = getenv("SHELL");
    FILE *fh = NULL;
    if(signal(SIGWINCH, handle_sigwinch) == SIG_ERR)
        perror("signal failed");
    if(argv[1] != NULL)
        ofname = argv[1];
    if(shellenv != NULL)
    {
        char *str = malloc(1 + strlen(shellenv));
        strcpy(str, shellenv);
        shell = shellenv = str;
    }
    succ = ioctl(STDIN_FILENO, TIOCGWINSZ, &dim);
    if(succ >= 0)
    {
        tcgetattr(STDIN_FILENO, &oldtm);
        memcpy(&currtm, &oldtm, sizeof(struct termios));
        currtm.c_lflag &= ~(ECHO|ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &currtm);
        succ = openpty(&master, &slave, NULL, NULL, &dim);
        if(succ == 0)
        {
            pid = fork();
            if(pid > 0)
            {
                slaveterm = slave;
                if(shellenv != NULL)
                    free(shellenv);
                fh = fopen(ofname, "w");
                if(fh == NULL)
                    perror("fopen failed");
                else
                    succ = pass_data(pid, master, fh);
            }
            else if(pid < 0)
                perror("fork failed");
            else
            {
                close(master);
                setsid();
                dup2(slave, STDIN_FILENO);
                dup2(slave, STDOUT_FILENO);
                dup2(slave, STDERR_FILENO);
                if(ioctl(slave, TIOCSCTTY) == -1)
                    perror("ioctl TIOCSCTTY failed");
                close(slave);
                execl(shell, shell, (char*)NULL);
                perror("execl failed");
                exit(1);
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldtm);
    }
    return succ * -1;
}
