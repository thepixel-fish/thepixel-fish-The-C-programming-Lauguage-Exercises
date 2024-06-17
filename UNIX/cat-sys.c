#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdarg.h>

void error(char *fmt, ...);
void filecopy(int fd1, int fd2);

int main(int argc, char *argv[])
{
    int fd;

    if (argc == 1)
        filecopy(0, 1);
    while (--argc > 0){
        if ((fd = open(*++argv, O_RDONLY, 0)) == -1)
            error("cat: can't open %s", *argv);
        else
            filecopy(fd, 1);
    }
    return 0;
}

void filecopy(int fd1, int fd2)
{
    int n;
    char buf[BUFSIZ];

    while ((n = read(fd1, buf, BUFSIZ)) > 0)
        if (write(fd2, buf, n) != n)
            error("cat: write error");
    close(fd1);
}

void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);        //用一个参数替代变长参数表？自动va_arg(args)?
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}