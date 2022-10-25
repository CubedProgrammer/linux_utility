#include<stdio.h>
size_t fromhexdec(void *src, void *dest, int(*gch)(void*), int(*pch)(int, void*))
{
    size_t cnt = 0;
    char complete = 1;
    int b = 0;
    for(int ch = gch(src); ch >= 0; ch = gch(src))
    {
        complete = !complete;
        b <<= 4;
        if(ch >= 'a' && ch <= 'z')
            b |= ch - 'a' + 10;
        else if(ch >= 'A' && ch <= 'Z')
            b |= ch - 'A' + 10;
        else if(ch >= '0' && ch <= '9')
            b |= ch - '0';
        else
        {
            complete = !complete;
            b >>= 4;
        }
        if(complete)
        {
            pch(b, dest);
            b = 0;
            ++cnt;
        }
    }
    if(!complete)
        pch(b << 4, dest);
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
    fromhexdec(stdin, stdout, &vgetc, &vputc);
    return 0;
}
