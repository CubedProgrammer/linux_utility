#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    int succ = 0;
    FILE *fh = argl < 2 ? NULL : fopen(argv[1], "r");
    if(fh == NULL)
        fh = stdin;
    fseek(fh, 0, SEEK_END);
    size_t sz = ftell(fh);
    fseek(fh, 0, SEEK_SET);
    char *buf = malloc(sz + 1);
    if(buf == NULL)
    {
        perror("malloc failed");
        succ = 1;
    }
    else
    {
        fread(buf, 1, sz, fh);
        if(fh != stdin)
            fclose(fh);
        size_t lncnt = 0;
        buf[sz] = '\0';
        for(char *it = buf; it != buf + sz; ++it)
        {
            if(*it == '\n')
            {
                *it = '\0';
                ++lncnt;
            }
        }
        char **arr = malloc((lncnt + 1) * sizeof(*arr));
        if(arr == NULL)
        {
            perror("malloc failed");
            succ = 1;
        }
        else
        {
            lncnt = 0;
            char *start = buf;
            for(char *it = buf; it != buf + sz; ++it)
            {
                if(*it == '\0')
                {
                    arr[lncnt] = start;
                    start = it + 1;
                    ++lncnt;
                }
            }
            if(arr[lncnt - 1][0] == '\0')
                --lncnt;
            arr[lncnt] = NULL;
            succ = execvp(arr[0], arr) * -1;
            perror("execvp failed");
            free(arr);
        }
        free(buf);
    }
    return succ;
}
