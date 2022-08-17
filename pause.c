#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
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
        usleep(atoi(argv[1]) * 1000);
    return 0;
}
