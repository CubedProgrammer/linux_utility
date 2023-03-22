#include<stdio.h>
int jhash(const char *it)
{
    unsigned ch;
    unsigned h = 0;
    for(; *it != '\0'; ++it)
    {
        ch = *it;
        ch &= 255;
        if(ch > 239)
        {
            ch = (it[0] & 7) << 18 | (it[1] & 63) << 12 | (it[2] & 63) << 6 | it[3] & 63;
            it += 3;
        }
        else if(ch > 223)
        {
            ch = (it[0] & 15) << 12 | (it[1] & 63) << 6 | it[2] & 63;
            it += 2;
        }
        else if(ch > 191)
        {
            ch = (it[0] & 31) << 6 | it[1] & 63;
            ++it;
        }
        h *= 31;
        if(ch > 0xffff)
        {
            h += ch >> 10 | 0xd800;
            h *= 31;
            ch &= 0x3ff;
            ch |= 0xdc00;
        }
        h += ch;
    }
    return h;
}
int main(int argl, char *argv[])
{
    for(int i = 1; i < argl; ++i)
        printf("%d\n", jhash(argv[i]));
    return 0;
}
