#include<stdio.h>
#include<stdlib.h>
const char dl_loader[] __attribute__((section(".interp"))) = "/usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2";
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
    FILE *i = stdin, *o = stdout;
    if(argl > 1)
    {
        if(argv[1][0] != '-' && argv[1][1] != '\0')
            i = fopen(argv[1], "r");
        if(argl > 2)
        {
            ++argv;
            if(argv[1][0] != '-' && argv[1][1] != '\0')
                o = fopen(argv[1], "w");
        }
    }
    fromhexdec(stdin, stdout, &vgetc, &vputc);
    fclose(o);
    fclose(i);
    return 0;
}
