#include<stdio.h>

#ifndef BUFSIZE
#define BUFSIZE 100
#endif

char buf[BUFSIZE];
int bufp = 0;

char getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungerch: too many characters\n");
    else
        buf[bufp++] = c;
}
