#include<stdio.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    
    while (--argc > 0){
        fp = stdout;
        printf("file:%d", fp->_file);
        printf("flag:%d", fp->_flag);
        printf("tmpfname:%s", fp->_tmpfname);
    }
    return 0;
}