#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
void findscrt(char *buf)
{
    const char *scrtname = "Scrt1.o";
    char *env = getenv("SCRT");
    unsigned len;
    char keep = 1;
    if(env != NULL)
        strcpy(buf, env);
    else
    {
        const char *check = "/usr/lib/Scrt1.o";
        if(access(check, F_OK) == 0)
            strcpy(buf, check);
        else
        {
            check = "/usr/lib64/Scrt1.o";
            if(access(check, F_OK) == 0)
                strcpy(buf, check);
            else
            {
                FILE *fh = fopen("/etc/ld.so.conf", "r");
                while(keep && fscanf(fh, "%s", buf) == 1)
                {
                    len = strlen(buf);
                    buf[len] = '/';
                    strcpy(buf + len + 1, scrtname);
                    keep = access(buf, F_OK);
                }
                fclose(fh);
                if(keep)
                {
                    DIR *dh = opendir("/etc/ld.so.conf.d");
                    for(struct dirent *en = readdir(dh); keep && en != NULL; en = readdir(dh))
                    {
                        if(en->d_name[0] == '.' && (en->d_name[1] == '\0' || en->d_name[1] == '.'))
                            continue;
                        strcpy(buf, "/etc/ld.so.conf.d/");
                        strcpy(buf + 18, en->d_name);
                        fh = fopen(buf, "r");
                        if(fh == NULL)
                            perror("fopen failed");
                        else
                        {
                            while(keep && fscanf(fh, "%s", buf) == 1)
                            {
                                len = strlen(buf);
                                buf[len] = '/';
                                strcpy(buf + len + 1, scrtname);
                                keep = access(buf, F_OK);
                            }
                            fclose(fh);
                        }
                    }
                    closedir(dh);
                    if(!keep)
                        strcpy(buf, "-Wl,-e,main");
                }
            }
        }
        if(keep)
            printf("%s was found, please set it as value of SCRT environment variable.\n", buf);
        else
            puts("Scrt1.o was not found, your program may not run as expected.");
    }
}
int main(int argl, char *argv[])
{
    char scc[] = "cc";
    char scpp[] = "c++";
    char sCOMPILER[] = "COMPILER";
    char sCOMPILERPP[] = "COMPILERPP";
    char scrt[2601];
    char **nargv;
    char pp = argv[1][0] == '+' && argv[1][1] == '+' && argv[1][2] == '\0';
    char *compiler = scc, *varname = sCOMPILER;
    argl -= pp + 1;
    argv += pp + 1;
    if(pp)
    {
        compiler = scpp;
        varname = sCOMPILERPP;
    }
    nargv = malloc((argl + 4) * sizeof(*nargv));
    for(int i = 0; i < argl; ++i)
        nargv[i + 3] = argv[i];
    findscrt(scrt);
    nargv[2] = scrt;
    nargv[1] = "-shared";
    nargv[0] = getenv(varname);
    if(*nargv == NULL)
        nargv[0] = compiler;
    return execvp(*nargv, nargv);
}
