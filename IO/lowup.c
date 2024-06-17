#include<stdio.h>
#include<ctype.h>
#include<string.h>

int main(int argc, char *argv[])
{
    int c;

    if (strcmp(argv[0], "lower") == 0)                      //error1:   while ((c = getchar()) != EOF && c != '#')
        while ((c = getchar()) != EOF && c != '#')          //              if (strcmp(argv[0], "lower") == 0) 的顺序无法执行
            putchar(tolower(c));                            
    else
        while ((c = getchar()) != EOF && c != '#')
            putchar(toupper(c));
    return 0;
}