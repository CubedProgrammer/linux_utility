#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
const char *search[30] = {"/usr/include", "/usr/local/include", NULL};
char findinc(char *restrict buf, const char *name, char plusplus)
{
    DIR *dh;
    struct dirent *en;
    unsigned len;
    char *fname, succ = 0;
    for(const char **dir = search; succ == 0 && *dir != NULL; ++dir)
    {
        dh = opendir(*dir);
        if(dh == NULL)
        {
            fprintf(stderr, "Directory %s", *dir);
            perror(" could not be opened");
        }
        else
        {
            for(en = readdir(dh); succ == 0 && en != NULL; en = readdir(dh))
            {
                fname = en->d_name;
                if(strcmp(fname, name) == 0)
                {
                    strcpy(buf, *dir);
                    len = strlen(buf);
                    buf[len] = '/';
                    strcpy(buf + len + 1, fname);
                    succ = 1;
                    puts(fname);
                }
            }
            closedir(dh);
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
    char space[16];
    char *fcarr[16];
    const char *nxtarr[16];
    size_t tmplen, pathnxt, fszarr[16];
    unsigned fcnt = 1;
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
            for(nxt += 8; *nxt == ' '; ++nxt);
            for(endl = nxt; *endl != '\n'; ++endl);
            memcpy(incf, nxt + 1, endl - nxt - 2);
            incf[endl - nxt - 2] = '\0';
            nxtarr[fcnt - 1] = endl;
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
    if(argv[1] == NULL)
        puts("Specify a file name.");
    else
    {
        searchf(argv[1], pp);
    }
    return succ;
}
