#include<stdio.h>
#include<string.h>
#include<sys/select.h>
#include<termios.h>
#include<time.h>
#include<unistd.h>
void tick(struct timespec start)
{
    struct timespec curr;
    struct timeval interval, icpy;
    char cbuf[31];
    unsigned hour, minute, second;
    unsigned millis;
    fd_set fds;
    char ch = 0;
    int ready = 0;
    interval.tv_sec = 0;
    interval.tv_usec = 1000;
    fputs("00:00:00.000\r", stdout);
    fflush(stdout);
    for(; ch != 27; ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &icpy))
    {
        clock_gettime(CLOCK_REALTIME, &curr);
        second = curr.tv_sec - start.tv_sec - (start.tv_sec > curr.tv_sec);
        minute = second / 60 % 60;
        hour = second / 3600;
        second %= 60;
        millis = curr.tv_nsec - start.tv_nsec;
        millis += (start.tv_nsec > curr.tv_nsec) * 1000000000;
        millis /= 1000000;
        printf("%02u:%02u:%02u.%03u\r", hour, minute, second, millis);
        fflush(stdout);
        if(ready)
        {
            ch = getchar();
            if(ch == '\n')
                putchar('\n');
        }
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        icpy = interval;
    }
    putchar('\n');
}
int main(int argl, char *argv[])
{
    struct termios old, curr;
    struct timespec ts;
    tcgetattr(STDIN_FILENO, &old);
    curr = old;
    curr.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &curr);
    clock_gettime(CLOCK_REALTIME, &ts);
    tick(ts);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return 0;
}
