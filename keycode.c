#include<stdio.h>
#include<termios.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    unsigned char cbuf[8];
    char exitch = 27;
    unsigned len = 0;
    struct termios old, curr;
    tcgetattr(STDIN_FILENO, &old);
    curr = old;
    curr.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &curr);
    if(argl > 1)
        exitch = argv[1][0];
    while(len != 1 || cbuf[0] != exitch)
    {
        len = read(STDIN_FILENO, cbuf, sizeof cbuf);
        for(unsigned i = 0; i < len; ++i)
            printf("%02X", (unsigned)cbuf[i]);
        putchar('\n');
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return 0;
}
