#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    int succ = 0;
    int pid, status;
    char compiler[2601];
    char oname[2601];
    char *fname = argv[1];
    const char *env = getenv("COMPILERPP");
    size_t len;
    if(env == NULL)
        env = "c++";
    strcpy(compiler, env);
    strcpy(oname, fname);
    len = strlen(oname);
    oname[len - 3] = 'o';
    oname[len - 2] = 'u';
    oname[len - 1] = 't';
    pid = fork();
    if(pid == 0)
        exit(execlp(compiler, compiler, "-std=c++20", "-o", oname, fname, (char*)NULL));
    else
    {
        waitpid(pid, &status, 0);
        pid = fork();
        if(pid == 0)
            exit(execlp(oname, oname, (char*)NULL));
        else
            waitpid(pid, &status, 0);
            succ = WEXITSTATUS(status);
    }
    return succ;
}
