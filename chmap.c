#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#define ring do{putchar('\a');fflush(stdout);}while(0)
int readkey(void)
{
    char cbuf[4];
    int cnt = read(STDIN_FILENO, cbuf, sizeof cbuf);
    int k = 0;
    for(int i = 0; i < cnt; ++i)
        k = k << 8 | cbuf[i];
    return k;
}
void putunic(int ch)
{
    char cbuf[5] = {0, 0, 0, 0, 0};
    if(ch < 32)
        cbuf[0] = ' ';
    else if(ch < 127)
        cbuf[0] = ch;
    else if(ch < 161)
        cbuf[0] = ' ';
    else if(ch < 2048)
    {
        cbuf[0] = ch >> 6 | 192;
        cbuf[1] = ch & 63 | 128;
    }
    else if(ch < 65536)
    {
        if(ch > 53247 && ch < 57344)
            cbuf[0] = ' ';
        else
        {
            cbuf[0] = ch >> 12 | 224;
            cbuf[1] = ch >> 6 & 63 | 128;
            cbuf[2] = ch & 63 | 128;
        }
    }
    else
    {
        cbuf[0] = ch >> 18 | 240;
        cbuf[1] = ch >> 12 & 63 | 128;
        cbuf[2] = ch >> 6 & 63 | 128;
        cbuf[3] = ch & 63 | 128;
    }
    fputs(cbuf, stdout);
}
void display(int start)
{
    start <<= 4;
    for(unsigned i = 0; i < 16; ++i)
    {
        for(unsigned j = 0; j < 16; ++j)
            putunic(start + i * 16 + j + 1);
        putchar('\n');
    }
    fflush(stdout);
}
int main(int argl, char *argv[])
{
    struct termios o, n;
    int curr = 0;
    if(argv[1] != NULL)
        curr = atoi(argv[1]);
    tcgetattr(STDIN_FILENO, &o);
    n = o;
    n.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &n);
    display(curr);
    for(int ch = readkey(); ch != 27; ch = readkey())
    {
        switch(ch)
        {
            case 0x1b5b41:
                if(curr > 0)
                    --curr;
                break;
            case 0x1b5b42:
                ++curr;
                break;
            case 0x1b5b357e:
                if(curr > 16)
                    curr -= 16;
                else
                    curr = 0;
                break;
            case 0x1b5b367e:
                curr += 16;
                break;
            default:
                ring;
        }
        fputs("\033\13316F", stdout);
        display(curr);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &o);
    return 0;
}
