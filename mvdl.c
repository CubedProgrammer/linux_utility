#include<dirent.h>
#include<linux/limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
typedef struct file_entry
{
    char *name;
    long mod;
}file_entry;
void swap_files(file_entry *a, file_entry *b)
{
    file_entry tmp = *a;
    *a = *b;
    *b = tmp;
}
void insert_file(file_entry *arr, size_t len, file_entry file)
{
    arr[len] = file;
    for(size_t up; len > 0; len >>= 1)
    {
        up = len - 1;
        up >>= 1;
        if(!(arr[up].mod < arr[len].mod))
            swap_files(arr + up, arr + len);
        --len;
    }
}
void delete_file(file_entry *arr, size_t len)
{
    size_t curr = 0;
    size_t lc, rc;
    size_t chosen;
    char should_swap;
    --len;
    swap_files(arr, arr + len);
    while(curr < len)
    {
        lc = (curr << 1) + 1;
        rc = (curr << 1) + 2;
        chosen = lc;
        should_swap = 0;
        if(lc < len)
        {
            if(arr[curr].mod > arr[lc].mod)
                should_swap = 1;
            if(rc < len)
            {
                if(arr[rc].mod < arr[lc].mod)
                    chosen = rc;
                if(arr[curr].mod > arr[rc].mod)
                    should_swap = 1;
            }
            if(should_swap)
                swap_files(arr + curr, arr + chosen);
        }
        curr = chosen;
    }
}
file_entry *get_files(char *d, size_t dlen, size_t len, unsigned *found)
{
    file_entry *arr = NULL;
    char *start = d + dlen + 1;
    DIR *handle = opendir(d);
    if(handle != NULL)
    {
        arr = malloc(len * sizeof(*arr));
        if(arr != NULL)
        {
            size_t cnt = 0;
            size_t slen;
            char *tmpstr;
            struct stat fdat;
            file_entry f;
            d[dlen] = '/';
            for(struct dirent *en = readdir(handle); en != NULL; en = readdir(handle))
            {
                if(en->d_type == DT_REG)
                {
                    slen = strlen(en->d_name);
                    tmpstr = malloc(slen + 1);
                    if(tmpstr != NULL)
                    {
                        strcpy(tmpstr, en->d_name);
                        strcpy(start, tmpstr);
                        stat(d, &fdat);
                        f.name = tmpstr;
                        f.mod = fdat.st_mtime;
                        if(cnt == len)
                        {
                            if(f.mod > arr[0].mod)
                            {
                                delete_file(arr, cnt);
                                --cnt;
                            }
                        }
                        if(cnt < len)
                        {
                            insert_file(arr, cnt, f);
                            ++cnt;
                        }
                    }
                }
            }
            *found = cnt;
            for(; cnt > 0; --cnt)
                delete_file(arr, cnt);
        }
        closedir(handle);
    }
    return arr;
}
int mvdl(unsigned cnt, char **first, char **last, const char *dir)
{
    int succ = 1;
    char dlbuf[PATH_MAX];
    const char *envstr = getenv("HOME");
    if(envstr != NULL)
    {
        size_t len = strlen(envstr);
        memcpy(dlbuf, envstr, len);
        dlbuf[len] = '/';
        strcpy(dlbuf + len + 1, "Downloads");
        len += 10;
        file_entry *arr = get_files(dlbuf, len, cnt, &cnt);
        if(arr != NULL)
        {
            char destbuf[PATH_MAX];
            char *fstart = dlbuf + len + 1;
            size_t destlen = dir == NULL ? 0 : strlen(dir);
            char *dstart = destbuf + destlen + 1;
            const char *dest;
            int mvsucc = 0;
            if(dir != NULL)
            {
                memcpy(destbuf, dir, destlen);
                destbuf[destlen] = '/';
            }
            for(unsigned i = 0; (first != last || dir != NULL) && i < cnt; ++i, ++first)
            {
                strcpy(fstart, arr[i].name);
                if(dir == NULL)
                    dest = *first;
                else
                {
                    strcpy(dstart, arr[i].name);
                    dest = destbuf;
                }
                mvsucc = rename(dlbuf, dest);
                if(mvsucc)
                {
                    fprintf(stderr, "renaming %s to %s", dlbuf, dest);
                    perror(" failed");
                }
                free(arr[i].name);
            }
            free(arr);
            succ = 0;
        }
    }
    else
        fputs("HOME environment variable is undefined\n", stderr);
    return succ;
}
int main(int argl, char *argv[])
{
    int succ = 0;
    const char *dir = NULL;
    if(argl == 1)
    {
        printf("Usage: %s N FILES...\n", argv[0]);
        printf("Usage: %s N DIRECTORY\n", argv[0]);
        puts("Moves N files from your downloads folder.");
    }
    else
    {
        unsigned cnt = atoi(argv[1]);
        if(cnt == 0)
        {
            fprintf(stderr, "%s is not a valid quantity\n", argv[1]);
            succ = 1;
        }
        else
        {
            if(argl == 2)
                dir = ".";
            else
            {
                struct stat dirdat;
                int statret = stat(argv[2], &dirdat);
                if(statret == 0)
                {
                    if(S_ISDIR(dirdat.st_mode))
                        dir = argv[2];
                }
            }
            succ = mvdl(cnt, argv + 2, argv + argl, dir);
        }
    }
    return succ;
}
