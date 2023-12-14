#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
struct lcg
{
    long unsigned slope, yint;
    long unsigned seed;
};
union dq
{
    long i;
    double f;
};
long unsigned next(struct lcg *generator)
{
    return generator->seed = generator->seed * generator->slope + generator->yint;
}
int main(int argl, char *argv[])
{
    struct lcg gen;
    FILE *fh = fopen("/dev/random", "r");
    union dq dummy;
    long unsigned upper, lower = 0;
    unsigned cnt = 1;
    fread(&gen.seed, 1, sizeof(gen.seed), fh);
    gen.slope = time(NULL);
    gen.yint = getpid() * getppid();
    fclose(fh);
    switch(argl)
    {
        default:
            cnt = atoi(argv[3]);
        case 3:
            lower = strtoul(argv[1], NULL, 10);
            ++argv;
        case 2:
            upper = strtoul(argv[1], NULL, 16);
            for(unsigned i = 0; i < cnt; ++i)
            {
                dummy.i = next(&gen) % (upper - lower) + lower;
                printf("%lu\n", dummy.i);
            }
            break;
        case 1:
            dummy.i = next(&gen) >> 12 | 0x3ff0000000000000;
            printf("%.12f\n", dummy.f - 1);
            break;
    }
    return 0;
}
