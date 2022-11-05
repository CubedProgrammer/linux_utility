#include<stdio.h>
#include<sys/select.h>
#include<termios.h>
#include<time.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    struct timespec tm;
    struct timeval tv;
    struct termios old, curr;
    fd_set fds;
    long sec;
    int ready;
    int h, m, s;
    tcgetattr(STDIN_FILENO, &old);
    curr = old;
    curr.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &curr);
    for(ready = 0; ready == 0; ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv))
    {
        clock_gettime(CLOCK_REALTIME, &tm);
        sec = tm.tv_sec - 1;
        h = sec / 3600 % 24;
        m = sec / 60 % 60;
        s = sec % 60;
        printf("%02d:%02d:%02d\r", h, m, s);
        fflush(stdout);
        tv.tv_sec = 0;
        tv.tv_usec = 1000000 - tm.tv_nsec / 1000;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    putchar('\n');
    return ready == -1;
}
