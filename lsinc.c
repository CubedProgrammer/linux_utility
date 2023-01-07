#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
const char *search[30] = {"/usr/include", "/usr/local/include", NULL};
char findinc(char *restrict buf, const char *name, char plusplus)
{
    unsigned len;
    struct stat fdat;
    char *fname, succ = 0;
    for(const char **dir = search; succ == 0 && *dir != NULL; ++dir)
    {
        len = strlen(*dir);
        memcpy(buf, *dir, len);
        buf[len] = '/';
        strcpy(buf + len + 1, name);
        if(access(buf, F_OK) == 0)
        {
            stat(buf, &fdat);
            puts(buf);
            succ = S_ISREG(fdat.st_mode) != 0;
        }
    }
    return succ;
}
char *rdfile(const char *name, size_t *szptr)
{
    struct stat fdat;
    stat(name, &fdat);
    *szptr = fdat.st_size;
    char *fcont = malloc(*szptr + 1);
    if(fcont == NULL)
        perror("malloc failed");
    else
    {
        FILE *fh = fopen(name, "r");
        if(fh == NULL)
            perror("fopen failed");
        else
        {
            *szptr = fread(fcont, 1, *szptr, fh);
            fcont[*szptr] = '\0';
            fclose(fh);
        }
    }
    return fcont;
}
void searchf(const char *fname, char pp)
{
    const char *nxt, *endl;
    const char *incdir = "#include";
    char incf[91], path[2601];
    char endch, space[16];
    char *fcarr[16];
    const char *nxtarr[16];
    size_t tmplen, pathnxt, fszarr[16];
    unsigned fcnt = 1;
    puts(fname);
    nxtarr[0] = fcarr[0] = rdfile(fname, fszarr);
    memset(space, ' ', sizeof space);
    strcpy(path, fname);
    pathnxt = strlen(path);
    while(fcnt > 0)
    {
        nxt = strstr(nxtarr[fcnt - 1], incdir);
        if(nxt == NULL)
        {
            free(fcarr[fcnt - 1]);
            --fcnt;
            for(--pathnxt; pathnxt > 0 && path[pathnxt] != '\0'; --pathnxt);
        }
        else
        {
            if(nxt[-1] != '\n')
            {
                nxtarr[fcnt - 1] = nxt + 8;
                continue;
            }
            for(nxt += 8; *nxt == ' '; ++nxt);
            if(*nxt == 60)
                endch = 62;
            else if(*nxt == '"')
                endch = '"';
            else
            {
                nxtarr[fcnt - 1] = nxt + 1;
                continue;
            }
            for(endl = nxt; *endl != endch; ++endl);
            memcpy(incf, nxt + 1, endl - nxt - 1);
            incf[endl - nxt - 1] = '\0';
            nxtarr[fcnt - 1] = endl + 1;
            fwrite(space, 1, fcnt, stdout);
            if(findinc(path + pathnxt + 1, incf, pp))
            {
                tmplen = strlen(path + pathnxt + 1) + 1;
                fcarr[fcnt] = rdfile(path + pathnxt + 1, fszarr + fcnt);
                nxtarr[fcnt] = fcarr[fcnt];
                ++fcnt;
                pathnxt += tmplen;
            }
            else
                printf("%s could not be found\n", incf);
        }
    }
}
int main(int argl, char *argv[])
{
    int succ = 0;
    char pp = 0;
    unsigned dircnt = 2;
    if(argv[1] == NULL)
        puts("Specify a file name.");
    else
    {
        pp = argv[1][0] == '+' && argv[1][1] == '+' && argv[1][2] == '\0';
        for(argv += pp + 1; *argv != NULL; ++argv)
        {
            if(argv[0][0] == '-')
            {
                if(argv[0][1] == 'I')
                    search[dircnt++] = argv[0] + 2;
            }
            else
                searchf(*argv, pp);
        }
    }
    return succ;
}
