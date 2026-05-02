#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ARGSYMBOL '$'
typedef struct CharArray
{
    char *arr;
    size_t len;
    size_t cap;
}CharArray;
typedef struct String
{
    char *arr;
    size_t len;
}String;
int init_char_array(CharArray *arr)
{
    arr->cap = 16;
    arr->arr = malloc(arr->cap * sizeof(*arr));
    arr->len = 0;
    return arr->arr == NULL;
}
int append_char_array(CharArray *arr, const char *buf, size_t blen)
{
    int succ = 0;
    char *tmp;
    size_t ncap = arr->cap << 1;
    if(arr->len + blen > arr->cap)
    {
        for(; ncap < arr->len + blen; ncap <<= 1);
        tmp = realloc(arr->arr, ncap * sizeof(*arr->arr));
        if(tmp == NULL)
        {
            perror("realloc failed");
            succ = 1;
        }
        else
        {
            arr->cap = ncap;
            arr->arr = tmp;
        }
    }
    if(succ == 0)
    {
        memcpy(arr->arr + arr->len, buf, blen);
        arr->len += blen;
    }
    return succ;
}
int final_truncate(int memerr, CharArray *arr, String *s)
{
    if(memerr)
        free(arr->arr);
    else
    {
        s->arr = realloc(arr->arr, arr->len);
        if(s->arr == NULL && arr->len != 0)
        {
            perror("final realloc for shrinking failed");
            free(arr->arr);
        }
        else
        {
            s->len = arr->len;
        }
    }
}
String read_line(FILE *handle)
{
    CharArray arr;
    String s = {NULL, 0};
    char c;
    int memerr = 0;
    if(init_char_array(&arr) == 0)
    {
        for(int ch = fgetc(handle); !memerr && ch != EOF && ch != '\n'; ch = fgetc(handle))
        {
            c = ch;
            memerr = append_char_array(&arr, &c, 1);
        }
    }
    final_truncate(memerr, &arr, &s);
    return s;
}
String read_file(FILE *handle)
{
    char cbuf[512];
    char *tmp;
    int me = 0;
    CharArray arr;
    String s = {NULL, 0};
    if(init_char_array(&arr) == 0)
    {
        for(size_t cnt = fread(cbuf, 1, sizeof(cbuf), handle); !me && cnt != 0; cnt = fread(cbuf, 1, sizeof(cbuf), handle))
        {
            me = append_char_array(&arr, cbuf, cnt);
        }
    }
    final_truncate(me, &arr, &s);
    return s;
}
CharArray dollar_indices(String template)
{
    CharArray arr;
    size_t offset;
    const char *dollarp = memchr(template.arr, ARGSYMBOL, template.len);
    int memerr = init_char_array(&arr);
    while(!memerr && dollarp != NULL)
    {
        offset = dollarp - template.arr;
        memerr = append_char_array(&arr, (const char*)&offset, sizeof(offset));
        dollarp = memchr(dollarp + 2, ARGSYMBOL, template.len - offset - 2);
    }
    if(memerr)
    {
        perror("dollar_indices: inserting to character array failed");
        free(arr.arr);
        arr.arr = NULL;
        arr.len = 0;
        arr.cap = 0;
    }
    else if(arr.arr != NULL && arr.len == 0)
    {
        free(arr.arr);
        fprintf(stderr, "There must be at least one %c in your template\n", ARGSYMBOL);
    }
    return arr;
}
void generate_single(CharArray ind, String template, String args, FILE *out)
{
    const size_t *ptr = (const size_t*)ind.arr;
    size_t len = ind.len / sizeof(*ptr);
    size_t last = 0;
    size_t off;
    char argind;
    size_t commas[80];
    size_t commacnt = 0;
    size_t back = 0;
    char esc = 0;
    for(size_t i = 0; commacnt < 80 && i < args.len; ++i)
    {
        if(back > 0)
            args.arr[i - back] = args.arr[i];
        switch(args.arr[i])
        {
        case',':
            if(!esc)
            {
                commas[commacnt] = i - back;
                ++commacnt;
                ++back;
            }
            break;
        case'\\':
            if(esc)
                esc = 0;
            else
            {
                esc = 1;
                ++back;
            }
            break;
        default:
            esc = 0;
            break;
        }
    }
    commas[commacnt] = args.len - back;
    ++commacnt;
    if(ptr[len - 1] == template.len - 1)
        --len;
    for(size_t i = 0; i < len; ++i)
    {
        off = ptr[i];
        fwrite(template.arr + last, 1, off - last, out);
        argind = template.arr[off + 1];
        if(argind == ARGSYMBOL)
            fputc(ARGSYMBOL, out);
        else
        {
            argind -= '0';
            if(argind < commacnt)
            {
                size_t start = argind == 0 ? 0 : commas[argind - 1];
                fwrite(args.arr + start, 1, commas[argind] - start, out);
            }
        }
        last = off + 2;
    }
    fwrite(template.arr + last, 1, template.len - last, out);
}
int generate(const char *template, const char *args, char**output, size_t outlen)
{
    size_t index = 0;
    FILE *fout = NULL;
    FILE *thandle = fopen(template, "r");
    int succ = 0;
    if(thandle == NULL)
    {
        fprintf(stderr, "opening %s", template);
        perror(" failed");
        succ = 1;
    }
    else
    {
        String tstr = read_file(thandle);
        CharArray indices = dollar_indices(tstr);
        fclose(thandle);
        if(indices.len != 0 && tstr.len != 0)
        {
            FILE *argh = fopen(args, "r");
            if(argh == NULL)
            {
                fprintf(stderr, "opening %s", args);
                perror(" failed");
                succ = 1;
            }
            else
            {
                String argln = read_line(argh);
                if(index == outlen)
                    fout = stdout;
                else
                    fout = fopen(output[0], "w");
                while(argln.len > 0)
                {
                    if(fout != NULL)
                    {
                        generate_single(indices, tstr, argln, fout);
                        free(argln.arr);
                        if(index < outlen)
                        {
                            if(index < outlen - 1)
                            {
                                fclose(fout);
                                ++index;
                                fout = fopen(output[index], "w");
                            }
                        }
                    }
                    else
                    {
                        fprintf(stderr, "opening %s", args);
                        perror(" failed");
                    }
                    argln = read_line(argh);
                }
                if(fout != NULL && fout != stdout)
                    fclose(fout);
                fclose(argh);
            }
            free(indices.arr);
            free(tstr.arr);
        }
        else if(tstr.len != 0)
            free(tstr.arr);
        else if(indices.len != 0)
            free(indices.arr);
    }
    return succ;
}
int main(int argl, char *argv[])
{
    int succ = 0;
    if(argl > 2)
        succ = generate(argv[1], argv[2], argv + 3, argl - 3);
    else
    {
        fputs("Specify at least two files, one for the template and one for the list of arguments\n", stderr);
        succ = 1;
    }
    return succ;
}
