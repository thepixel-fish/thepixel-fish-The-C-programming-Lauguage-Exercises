//将文件1复制副本
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdarg.h>

#define PERMS 0666

void error(char *, ...);

int main(int argc, char *argv[])
{
    int fd1, fd2, n;
    char buf[BUFSIZ];

    if (argc != 3)
        error("Usage: cp from to");
    if ((fd1 = open(argv[1], O_RDONLY, 0)) == -1)
        error("cp: can't open %s", argv[1]);
    if ((fd2 = creat(argv[2], PERMS)) == -1)
        error("cp: creat %s,mode %03o", argv[2], PERMS);
    while ((n = read(fd1, buf, BUFSIZ)) > 0)        //read未到文件尾>0即可
        if (write(fd2, buf, n) != n)
            error("cp: write error\n");
}

//打印一个错误信息，然后终止
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