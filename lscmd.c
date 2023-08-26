#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
struct trie
{
    struct trie *children[256];
};
void init_trie(struct trie *trie)
{
    memset(trie->children, 0, sizeof(trie->children));
}
int insert_cmd(struct trie *trie, const char *str)
{
    int succ;
    char c;
    struct trie *n = trie;
    succ = 0;
    for(const char *it = str; succ == 0 && *it != '\0'; ++it)
    {
        c = *it;
        if(n->children[c] == NULL)
        {
            n->children[c] = malloc(sizeof(struct trie));
            if(n->children[c] == NULL)
                succ = -1;
            else
                init_trie(n->children[c]);
        }
        n = n->children[c];
    }
    if(succ == 0)
        n->children[0] = trie;
    return succ;
}
void display_trie(const struct trie *trie, const char *prefix)
{
    char unsigned str[50];
    const struct trie *ls[50];
    size_t len = 1;
    if(prefix != NULL)
    {
        for(const char *it = prefix; trie != NULL && *it != '\0'; ++it)
            trie = trie->children[*it];
    }
    if(trie != NULL)
    {
        if(trie->children[0] != NULL && prefix != NULL)
            puts(prefix);
        memset(str, 0, sizeof str);
        str[0] = 1;
        str[1] = 0;
        ls[0] = trie;
        while(str[0] != 0)
        {
            if(str[len - 1] == 0)
            {
                --len;
                if(len > 0)
                    ++str[len - 1];
            }
            else if(ls[len - 1]->children[str[len - 1]] != NULL)
            {
                str[len] = '\0';
                ls[len] = ls[len - 1]->children[str[len - 1]];
                if(ls[len]->children[0] != NULL)
                {
                    if(prefix != NULL)
                        fputs(prefix, stdout);
                    puts((char*)str);
                }
                ++str[len];
                ++len;
            }
            else
                ++str[len - 1];
        }
    }
}
int main(int argl, char *argv[])
{
    int succ = 0;
    char exepath[2601];
    char *pp = getenv("PATH"), end = 0;
    size_t len = strlen(pp);
    char *path = malloc(len + 1);
    struct trie trie;
    char *colon, *dirstart, *exename;
    DIR *d;
    struct dirent *en;
    init_trie(&trie);
    strcpy(path, pp);
    dirstart = path;
    for(colon = strchr(path, ':'); end == 0; colon = strchr(dirstart, ':'))
    {
        if(colon == NULL)
        {
            for(colon = dirstart; *colon != '\0'; ++colon);
            end = 1;
        }
        else
            *colon = '\0';
        d = opendir(dirstart);
        if(d == NULL)
        {
            fprintf(stderr, "Directory %s", dirstart);
            perror(" could not be opened");
        }
        else
        {
            en = readdir(d);
            len = colon - dirstart;
            strcpy(exepath, dirstart);
            exepath[len] = '/';
            while(en != NULL)
            {
                exename = en->d_name;
                strcpy(exepath + len + 1, exename);
                if(en->d_type == DT_LNK || en->d_type == DT_REG && access(exepath, X_OK) == 0)
                {
                    if(insert_cmd(&trie, exename) == -1)
                    {
                        fprintf(stderr, "%s ", exename);
                        perror("could not be inserted");
                    }
                }
                en = readdir(d);
            }
            closedir(d);
        }
        if(end == 0)
            *colon = ':';
        else
            --colon;
        dirstart = colon + 1;
    }
    if(argl > 2)
    {
        for(int i = 1; i < argl; ++i)
            display_trie(&trie, argv[i]);
    }
    else
        display_trie(&trie, argv[1]);
    free(path);
    return succ;
}
