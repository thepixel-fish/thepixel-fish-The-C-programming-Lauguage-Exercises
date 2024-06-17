#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAXWORD 100

int main(int argc, char *argv[])
{
    int c;
    char num_sys;

    printf("which one u prefer to print?(8or 16):");
    num_sys = getchar();
    if (num_sys == "8")
        while((c = getchar()) != EOF && c != '#')
                if (!isgraph(c))
                    printf("%u", c);
                else
                    putchar(c);
    else if (num_sys == "16")
        while ((c = getchar()) != EOF && c != '#')
             if (!isgraph(c))
                    printf("%X", c);
                else
                    putchar(c);
    else 
        while ((c = getchar()) != EOF && c != '#')
            putchar(c);
}