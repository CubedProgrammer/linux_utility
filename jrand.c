#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#define SLOPE 25214903917
#define YINT 11
#define MOD 281474976710656
long setSeed(long unsigned seed)
{
    return(seed ^ SLOPE) % MOD;
}
long next(long unsigned seed)
{
    return(seed * SLOPE + YINT) % MOD;
}
int main(int argl, char *argv[])
{
    if(argl == 1)
    {
        puts("Enter a seed.");
        return 1;
    }
    else
    {
        struct termios old, curr;
        unsigned cnt = 0;
        long seed = setSeed(strtoll(argv[1], NULL, 10));
        if(argv[2] != NULL)
            cnt = atoi(argv[2]);
        if(cnt == 0)
        {
            tcgetattr(STDIN_FILENO, &old);
            curr = old;
            curr.c_lflag &= ~(ECHO | ICANON);
            tcsetattr(STDIN_FILENO, TCSANOW, &curr);
            for(int ch = '\n'; ch == '\n'; ch = getchar())
            {
                printf("%ld\n", seed);
                seed = next(seed);
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &old);
        }
        else
        {
            for(unsigned i = 0; i < cnt; ++i)
            {
                printf("%ld\n", seed);
                seed = next(seed);
            }
        }
        return 0;
    }
}
