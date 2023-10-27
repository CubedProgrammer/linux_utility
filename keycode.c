#include<stdio.h>
#include<termios.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    unsigned char cbuf[8];
    char outbuf[17];
    const char digitarr[] = "0123456789ABCDEF";
    char exitch = 27;
    unsigned len = 0;
    struct termios old, curr;
    tcgetattr(STDIN_FILENO, &old);
    curr = old;
    curr.c_iflag &= ~(ICRNL | IXON);
    curr.c_lflag &= ~(ECHO | ICANON | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &curr);
    if(argl > 1)
        exitch = argv[1][0];
    while(len != 1 || cbuf[0] != exitch)
    {
        len = read(STDIN_FILENO, cbuf, sizeof cbuf);
        for(unsigned i = 0; i < len; ++i)
        {
            outbuf[i << 1] = digitarr[cbuf[i] >> 4];
            outbuf[(i << 1) + 1] = digitarr[cbuf[i] & 15];
        }
        outbuf[len << 1] = '\n';
        fwrite(outbuf, 1, (len << 1) + 1, stdout);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return 0;
}
