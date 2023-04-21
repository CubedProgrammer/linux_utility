#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argl, char *argv[])
{
    char str[31];
    unsigned off = 0, ind = 0;
    unsigned hc = atoi(argv[1]);
    for(; hc > 0; ++ind, hc /= 31)
        str[ind] = hc % 31;
    str[ind] = '\0';
    for(unsigned i = 0; i < ind >> 1; ++i)
        str[ind - i - 1] ^= str[i] ^= str[ind - i - 1] ^= str[i];
    if(ind > 1)
    {
        for(unsigned i = 0; i + 1 < ind; ++i)
        {
            while(str[i] && str[i + 1] <= 31)
            {
                --str[i];
                if(str[i] == 0 && i == off)
                    ++off;
                str[i + 1] += 31;
            }
        }
        puts(str + off);
    }
    return 0;
}
