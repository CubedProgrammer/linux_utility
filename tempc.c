#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>
int main(int argl, char *argv[])
{
    int succ = 0;
    char fmt[] = "/tmp/tempc%d";
    char tmpdir[2601];
    char cmd[2601];
    snprintf(tmpdir, sizeof(tmpdir), fmt, (int)getuid());
    if(argl > 1)
    {
        char mftpath[2601];
        int exitst;
        const char *fname = "main";
        char *env = getenv("HOME");
        char scriptpath[2601];
        strcpy(scriptpath, env);
        size_t len = strlen(scriptpath);
        strcpy(mftpath, scriptpath);
        strcpy(scriptpath + len, "/.tempc/");
        strcpy(mftpath + len, "/.ftemplates/.");
        strcpy(mftpath + len + 14, argv[1]);
        len += 8;
        strcpy(scriptpath + len, argv[1]);
        if(access(tmpdir, F_OK))
            succ = mkdir(tmpdir, 0755);
        if(succ == 0)
        {
            if(argl > 2)
                fname = argv[2];
            len = strlen(tmpdir);
            tmpdir[len] = '/';
            strcpy(tmpdir + len + 1, fname);
            len += strlen(fname) + 1;
            if(access(mftpath, F_OK) == 0)
            {
                sprintf(cmd, "mft %s %s", tmpdir, argv[1]);
                system(cmd);
            }
            tmpdir[len] = '.';
            strcpy(tmpdir + len + 1, argv[1]);
            env = getenv("EDITOR");
            if(env == NULL)
                env = "vi";
            sprintf(cmd, "%s %s", env, tmpdir);
            system(cmd);
            sprintf(cmd, "%s %s", scriptpath, tmpdir);
            exitst = system(cmd);
            exitst = WEXITSTATUS(exitst);
            printf("Process exited with status %d.\n", exitst);
            unlink(tmpdir);
        }
        else
        {
            fprintf(stderr, "Opening %s", tmpdir);
            perror(" failed");
        }
    }
    else
    {
        char rmfmt[] = "rm -fr %s";
        sprintf(cmd, rmfmt, tmpdir);
        system(cmd);
        puts("Enter the file type, followed by, optionally, the file name before the extension.");
        printf("%s c main\n", *argv);
    }
    return succ;
}
