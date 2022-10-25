#include<stdio.h>
size_t tohexdec(void *src, void *dest, int(*gch)(void*), int(*pch)(int, void*))
{
    size_t cnt = 0;
    unsigned char hi, lo;
    for(int ch = gch(src); ch >= 0; ch = gch(src))
    {
        hi = ch >> 4;
        lo = ch & 15;
        if(hi >= 10)
            hi += 'A' - 10;
        else
            hi += '0';
        if(lo >= 10)
            lo += 'A' - 10;
        else
            lo += '0';
        pch(hi, dest);
        pch(lo, dest);
        ++cnt;
    }
    return cnt;
}
static int vgetc(void *src)
{
    return fgetc((FILE*)src);
}
static int vputc(int ch, void *dest)
{
    return fputc(ch, (FILE*)dest);
}
int main(int argl, char *argv[])
{
    tohexdec(stdin, stdout, &vgetc, &vputc);
    return 0;
}
