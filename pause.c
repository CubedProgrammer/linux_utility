#include<stdio.h>
#include<string.h>
#include<termios.h>
#include<time.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    if(argl == 1)
    {
        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios tdat;
        tcgetattr(STDIN_FILENO, &tdat);
        tdat.c_lflag &= ~ECHO;
        tdat.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &tdat);
        fputs("Press any key to continue.", stdout);
        getchar();
        tdat.c_lflag |= ECHO;
        tdat.c_lflag |= ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &tdat);
        fputc('\n', stdout);
    }
    else
    {
        long n = 0;
        struct timespec tm;
        const char *it;
        for(it = argv[1]; *it >= '0' && *it <= '9'; ++it)
            n = n * 10 + *it - '0';
        n *= 1000000000;
        if(strcmp(it, "ms") == 0)
            n /= 1000;
        else if(strcmp(it, "us") == 0)
            n /= 1000000;
        else if(strcmp(it, "ns") == 0)
            n /= 1000000000;
        tm.tv_sec = n / 1000000000;
        tm.tv_nsec = n % 1000000000;
        nanosleep(&tm, NULL);
    }
    return 0;
}
