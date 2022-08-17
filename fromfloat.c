#include<stdio.h>
#include<stdlib.h>
union l_f
{
    unsigned i;
    float f;
};
int main(int argl, char *argv[])
{
    unsigned expcnt = 8, sigcnt = 23;
    union l_f dummy;
    unsigned tmp;
    unsigned exp, mant;
    char expstr[64], mantstr[64];
    for(int i = 1; i < argl; i++)
    {
        dummy.f = atof(argv[i]);
        tmp = dummy.i;
        exp = tmp >> sigcnt & ((1 << expcnt) - 1);
        mant = tmp & ((1 << sigcnt) - 1);
        for(unsigned i = 0; i < expcnt; i++)
            expstr[expcnt - i - 1] = '0' + (exp >> i & 1);
        expstr[expcnt] = '\0';
        for(unsigned i = 0; i < sigcnt; i++)
            mantstr[sigcnt - i - 1] = '0' + (mant >> i & 1);
        mantstr[sigcnt] = '\0';
        printf("0b%u\033\13332m%s\033\13334m%s\033\1330m is %f\n", tmp >> (expcnt + sigcnt) & 1, expstr, mantstr, dummy.f);
    }
    return 0;
}
