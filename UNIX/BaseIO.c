#include<stdio.h>
#include<unistd.h>

int main(void)
{
    char Buf[BUFSIZ];
    int n;

    while ((n = read(0, Buf, BUFSIZ)) > 0)
        write(1, Buf, n);
    return 0;
}

int getchar(void)       //单次读入一个字符
{
    char c;

    return (read(0, &c, 1) == 1) ? (unsigned char)c : EOF;
}

int getch(void)
{
    static char buf[BUFSIZ];
    static char *bufp = buf;
    static int n;

    if (n == 0){
        n = read(0, buf, sizeof buf);
        bufp = buf;
    }
    return (--n > 0) ? *++bufp : EOF;
}