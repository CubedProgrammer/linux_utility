int dig(char c)
{
    if(c >= '0' && c <= '9')
        return c - '0';
    else if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else
        return-1;
}
int main(int argl, char *argv[])
{
    int value = 0;
    int radix = 10;
    if(argl > 1)
    {
        const char *numstr = argv[1];
        if(*numstr == '0')
        {
            switch(*++numstr)
            {
                case'B':
                case'b':
                    radix = 2;
                    ++numstr;
                    break;
                case'X':
                case'x':
                    radix = 16;
                    ++numstr;
                    break;
                default:
                    radix = 8;
                    break;
            }
        }
        for(; *numstr != '\0'; ++numstr)
            value = value * radix + dig(*numstr);
    }
    return value;
}
