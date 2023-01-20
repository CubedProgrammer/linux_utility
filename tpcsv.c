#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void putlns(FILE *fh, char ***mat, size_t rc, size_t cc)
{
    --rc;
    for(size_t i = 0; i < cc; ++i)
    {
        for(size_t j = 0; j < rc; ++j)
        {
            fprintf(fh, "%s,", mat[i][j]);
            free(mat[i][j]);
        }
        fprintf(fh, "%s\n", mat[i][rc]);
        free(mat[i][rc]);
    }
}
size_t readlns(FILE *fh, char ****matp, size_t capa, size_t cc)
{
    char **tmpcol, ***mat = *matp;
    size_t len = 1, lnlen, lncapa;
    char *old, *ptr;
    char *tmpln, *ln;
    int ch = fgetc(fh);
    while(!feof(fh))
    {
        lnlen = 0;
        lncapa = 30;
        ln = malloc(lncapa);
        for(; !feof(fh) && ch != '\n'; ch = fgetc(fh))
        {
            if(lnlen == lncapa)
            {
                lncapa += lncapa >> 1;
                tmpln = malloc(lncapa);
                if(tmpln == NULL)
                    perror("malloc failed");
                memcpy(tmpln, ln, lnlen);
                free(ln);
                ln = tmpln;
            }
            ln[lnlen] = ch;
            ++lnlen;
        }
        if(len == capa)
        {
            capa += capa >> 1;
            for(size_t i = 0; i < cc; ++i)
            {
                tmpcol = malloc(capa * sizeof(*tmpcol));
                if(tmpcol == NULL)
                    perror("malloc failed");
                memcpy(tmpcol, mat[i], len * sizeof(char*));
                free(mat[i]);
                mat[i] = tmpcol;
            }
        }
        old = ln;
        for(size_t i = 0; i + 1 < cc; ++i)
        {
            for(ptr = old; *ptr != ','; ++ptr);
            mat[i][len] = malloc(ptr - old + 1);
            if(mat[i][len] == NULL)
                perror("malloc failed");
            memcpy(mat[i][len], old, ptr - old);
            mat[i][len][ptr - old] = '\0';
            old = ptr + 1;
        }
        mat[cc - 1][len] = malloc(ln + lnlen - old + 1);
        memcpy(mat[cc - 1][len], old, ln + lnlen - old);
        mat[cc - 1][len][ln + lnlen - old] = '\0';
        ++len;
        free(ln);
        ch = fgetc(fh);
    }
    *matp = mat;
    return len;
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl < 2)
    {
        puts("Transpose Comma Separated Values");
        puts("All arguments are files to transpose.");
    }
    else
    {
        const char *fname;
        FILE *fh;
        char ***mat, *ln;
        char *tmpln;
        size_t cc, len, capa;
        char ch, *ptr, *old;
        for(int fi = 1; fi < argl; ++fi)
        {
            fname = argv[fi];
            fh = fopen(fname, "r");
            if(fh == NULL)
            {
                fprintf(stderr, "File %s", fname);
                perror(" could not be opened");
                ++succ;
            }
            else
            {
                cc = 1;
                len = 0;
                capa = 40;
                ln = malloc(capa);
                for(ch = fgetc(fh); ch != '\n'; ch = fgetc(fh))
                {
                    if(ch == ',')
                        ++cc;
                    if(len == capa)
                    {
                        capa += capa >> 1;
                        tmpln = malloc(capa);
                        if(tmpln == NULL)
                            perror("malloc failed");
                        memcpy(tmpln, ln, len);
                        free(ln);
                        ln = tmpln;
                    }
                    ln[len] = ch;
                    ++len;
                }
                old = ln;
                capa = 40;
                mat = malloc(cc * sizeof(*mat));
                for(size_t i = 0; i < cc; ++i)
                {
                    mat[i] = malloc(capa * sizeof(*mat[i]));
                    if(mat[i] == NULL)
                        perror("malloc failed");
                }
                for(size_t i = 0; i + 1 < cc; ++i)
                {
                    for(ptr = old; *ptr != ','; ++ptr);
                    *mat[i] = malloc(ptr - old + 1);
                    if(*mat[i] == NULL)
                        perror("malloc failed");
                    memcpy(mat[i][0], old, ptr - old);
                    mat[i][0][ptr - old] = '\0';
                    old = ptr + 1;
                }
                mat[cc - 1][0] = malloc(ln + len - old + 1);
                memcpy(mat[cc - 1][0], old, ln + len - old);
                mat[cc - 1][0][ln + len - old] = '\0';
                free(ln);
                len = readlns(fh, &mat, capa, cc);
                fclose(fh);
                fh = fopen(fname, "w");
                if(fh == NULL)
                    perror("fopen for writing failed");
                else
                {
                    putlns(fh, mat, len, cc);
                    fclose(fh);
                }
            }
        }
    }
    return succ;
}
