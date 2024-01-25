#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
int execute(char *program, char *const*args)
{
    int succ = 0;
    int pid = fork();
    if(pid == 0)
    {
        char *rargs[6] = {program, NULL, NULL, NULL, NULL, NULL};
        for(char *const*it = args, **destit = rargs + 1; *it; *destit++ = *it++);
        execvp(program, rargs);
        exit(1);
    }
    else if(pid > 0)
    {
        int status;
        succ = waitpid(pid, &status, 0);
        if(succ != -1)
            succ = WEXITSTATUS(status);
    }
    else
        succ = -1;
    return succ;
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl < 3)
    {
        fputs("Put the name and extension of the file.\n", stderr);
        succ = 1;
    }
    else
    {
        char path[2048], dest[2048];
        char *name = strrchr(argv[1], '/');
        if(name == NULL)
            name = argv[1];
        else
            ++name;
        char *template = argv[2], *ext = argv[3];
        if(argl == 3)
            ext = template;
        char *home = getenv("HOME");
        strcpy(path, home);
        size_t pathlen = strlen(path), extlen = strlen(ext);
        strcpy(path + pathlen, "/.ftemplates/.");
        pathlen += 14;
        strcpy(path + pathlen, template);
        strcpy(dest, argv[1]);
        pathlen = strlen(argv[1]);
        dest[pathlen] = '.';
        ++pathlen;
        strcpy(dest + pathlen, ext);
        char replace[2048];
        strcpy(replace, "s/__NAME__/");
        pathlen = 11;
        strcpy(replace + pathlen, name);
        strcpy(replace + pathlen + strlen(name), "/g");
        char sedi[] = "-i";
        char cp[] = "cp";
        char sed[] = "sed";
        char *args[5] = {path, dest, NULL, NULL, NULL};
        int exv;
        if(access(dest, F_OK))
        {
            exv = execute(cp, args);
            if(exv == -1)
                fprintf(stderr, "Command %s failed to execute.\n", cp);
            else
                succ = exv;
            args[0] = sedi, args[1] = replace, args[2] = dest;
            exv = execute(sed, args);
            if(exv == -1)
                fprintf(stderr, "Command %s failed to execute.\n", sed);
            else
               succ = exv;
            memcpy(replace + 4, "TYPE", 4);
            strcpy(replace + pathlen, argv[2]);
            strcpy(replace + pathlen + extlen, "/g");
            exv = execute(sed, args);
            if(exv == -1)
                fprintf(stderr, "Command %s failed to execute.\n", sed);
            else
                succ = exv;
        }
        else
        {
            fprintf(stderr, "%s already exists.\n", dest);
            succ = 1;
        }
    }
    return succ;
}
