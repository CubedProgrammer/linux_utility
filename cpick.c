#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
void show(int colour)
{
    printf("\033\13348;2;%d;%d;%dm    \033\133m%06X\r", colour >> 16, colour >> 8 & 255, colour & 255, colour);
    fflush(stdout);
}
int main(int argl, char *argv[])
{
    struct termios old, curr;
    int colour = 0;
    int direction = 1;
    if(argl > 1)
        colour = strtoul(argv[1], NULL, 16);
    tcgetattr(STDIN_FILENO, &old);
    curr = old;
    curr.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &curr);
    show(colour);
    for(int ch = getchar(); ch != 'q'; ch = getchar())
    {
        switch(ch)
        {
            case'0':
                colour = 0;
                break;
            case'1':
                colour = 0xffffff;
                break;
            case'r':
                colour = colour & 0xffff | colour + (direction << 16) & 0xff0000;
                break;
            case'R':
                colour = colour & 0xffff | colour + (direction << 20) & 0xff0000;
                break;
            case'g':
                colour = colour & 0xff00ff | colour + (direction << 8) & 0xff00;
                break;
            case'G':
                colour = colour & 0xff00ff | colour + (direction << 12) & 0xff00;
                break;
            case'b':
                colour = colour & 0xffff00 | colour + direction & 0xff;
                break;
            case'B':
                colour = colour & 0xffff00 | colour + (direction << 4) & 0xff;
                break;
            case'x':
            case'X':
                direction *= -1;
                break;
            default:
                putchar('\a');
        }
        show(colour);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    putchar('\n');
    return 0;
}
