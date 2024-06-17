#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{

}

int get(int fd, long pos, char *buf, int n)
{
    if (lseek(fd, pos, 0) >= 0)
        return read(fd, buf, n);
    else
        return -1;
}