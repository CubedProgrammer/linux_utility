#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int install(const char *arg)
{
    int succ = 0;
    char dest[2601] = "/usr/local/lib/";
    FILE *fh = fopen(arg, "r");
    if(fh == NULL)
    {
        fprintf(stderr, "File %s", arg);
        perror(" could not be opened");
        succ = 1;
    }
    else
    {
        int fd;
        char buf[8192];
        const char *ptr, *lperiod = NULL;
        char ch, numerical, so = 0;
        char fini = 0;
        const char *slashp = strrchr(arg, '/');
        if(slashp != NULL)
            arg = slashp + 1;
        for(ptr = arg; *ptr != '\0'; ++ptr)
        {
            ch = *ptr;
            if(ch == '.')
            {
                if(lperiod != NULL)
                {
                    checkso:
                    so %= 3;
                    switch(so)
                    {
                        case 0:
                            if(lperiod + 3 == ptr && lperiod[1] == 's' && lperiod[2] == 'o')
                                ++so;
                            break;
                        case 1:
                        case 2:
                            for(numerical = 1, ++lperiod; numerical && lperiod != ptr; ++lperiod)
                                numerical = *lperiod >= '0' && *lperiod <= '9';
                            so += numerical;
                    }
                }
                lperiod = ptr;
            }
        }
        if(!fini)
        {
            fini = 1;
            goto checkso;
        }
        if(so)
            strcpy(dest + 15, arg);
        else
        {
            for(--ptr; *ptr != '.'; --ptr);
            memcpy(dest + 15, arg, ptr - arg);
            dest[11] = 'b';
            dest[12] = 'i';
            dest[13] = 'n';
            dest[ptr - arg + 15] = '\0';
        }
        fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0755);
        if(fd == -1)
        {
            fprintf(stderr, "File %s", dest);
            perror(" could not be opened");
            succ = 1;
        }
        else
        {
            size_t bc, sz = sizeof buf;
            for(bc = fread(buf, 1, sz, fh); bc == sz; bc = fread(buf, 1, sz, fh))
                write(fd, buf, sz);
            close(fd);
        }
        fclose(fh);
    }
    return succ;
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argv[1] == NULL)
        printf("%s file1 file2 file3 ...\n", *argv);
    else
    {
        for(int i = 1; i < argl; ++i)
            succ += install(argv[i]);
    }
    return succ;
}
