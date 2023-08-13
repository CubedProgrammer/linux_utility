#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    char remote[361];
    int succ = 0;
    const char *env = getenv("REMOTE_LOCATION");
    if(access(".remoteloc", F_OK) == 0)
    {
        FILE *fh = fopen(".remoteloc", "r");
        if(fh == NULL)
        {
            if(env != NULL)
                strcpy(remote, env);
            else
                remote[0] = '\0';
        }
        else
        {
            remote[fread(remote, 1, sizeof(remote) - 1, fh)] = '\0';
            fclose(fh);
        }
    }
    else if(env != NULL)
        strcpy(remote, env);
    else
        remote[0] = '\0';
    if(remote[0] == '\0')
    {
        succ = 1;
        puts("REMOTE_LOCATION environment variable or .remoteloc file is needed.");
    }
    else
    {
        char cmd[441];
        for(int i = 1; i < argl; ++i)
        {
            sprintf(cmd, "scp %s/%s %s", remote, argv[i], argv[i]);
            system(cmd);
        }
    }
    return succ;
}
