#include<stdio.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#else
#include<termios.h>
#include<unistd.h>
#endif
union f_i
{
    float f;
    unsigned i;
};
int rd(void);
void print(union f_i dummy)
{
    unsigned sign = dummy.i >> 31;
    unsigned exp = dummy.i >> 23;
    exp &= 0xff;
    unsigned mant = dummy.i & 0x7fffff;
    char expstr[9], mantstr[24];
    for(int i = 0; i < 8; i++)
        expstr[i] = '0' + (exp >> (7 - i) & 1);
    for(int i = 0; i < 23; i++)
        mantstr[i] = '0' + (mant >> (22 - i) & 1);
    expstr[8] = '\0';
    mantstr[23] = '\0';
    printf("\r0b%u\033\13332m%s\033\13333m%s\033\1330m is %e", sign, expstr, mantstr, dummy.f);
}
int main(int argl, char *argv[])
{
#ifdef _WIN32
    HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD cm;
    GetConsoleMode(ho, &cm);
    cm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(ho, cm);
#else
    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    struct termios curr;
    memcpy(&curr, &old, sizeof(struct termios));
    curr.c_lflag &= ~ECHO;
    curr.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &curr);
#endif
    unsigned i = 0;
    union f_i dummy;
    dummy.i = i;
    print(dummy);
    fputs("\r0b", stdout);
    unsigned pos = 0;
    for(int cmd = rd(); cmd != 'q'; cmd = rd())
    {
        switch(cmd)
        {
#ifdef _WIN32
            case-040:
#else
            case 033:
                rd();
#endif
                cmd = rd();
                if(cmd == 'C')
                {
                    if(pos < 31)
                        ++pos;
                    else
                        putchar('\a');
                }
                else if(cmd == 'D')
                {
                    if(pos > 0)
                        --pos;
                    else
                        putchar('\a');
                }
                else if(cmd == 'H')
                    pos = 0;
                else if(cmd == 'F')
                    pos = 31;
                else if(cmd == '1')
                {
                    rd();
                    rd();
                    cmd = rd();
                    if(cmd == 'C')
                    {
                        if(pos == 31)
                            putchar('\a');
                        else if(pos > 23)
                            pos = 31;
                        else
                            pos += 8;
                    }
                    else if(cmd == 'D')
                    {
                        if(pos == 0)
                            putchar('\a');
                        else if(pos < 8)
                            pos = 0;
                        else
                            pos -= 8;
                    }
                }
                break;
            case'E':
                pos = 1;
                break;
            case'M':
                pos = 9;
                break;
            case'f':
                dummy.i = ~dummy.i;
                break;
            case'e':
                dummy.i ^= 0x7f800000;
                break;
            case'm':
                dummy.i ^= 0x7fffff;
                break;
            case' ':
                dummy.i ^= 1 << (31 - pos);
                break;
            default:
                putchar('\a');
        }
        print(dummy);
        printf("\033\133%uG", 3 + pos);
    }
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
#endif
    putchar('\n');
    return 0;
}
int rd(void)
{
#ifdef _WIN32
    char c = getch();
#else
    char c = getchar();
#endif
    return c;
}
