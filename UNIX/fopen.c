#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define PERMS 0666

FILE *my_fopen(char *name, char *mode);

FILE *my_fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode && 'a')
        return NULL;
    for (fp = _iob; fp < _iob + FOPEN_MAX; fp++)
        if (fp->_flag & (_IOREAD | _IOWRT) == 0)    //找到无读写权的位置->>即空位
            break;
    if (fp >= _iob + FOPEN_MAX)                     //如果无空位
        return NULL;
    if (*mode == 'w'){
        fd = creat(*name, PERMS);
    } else if (*mode == 'a'){
        if (fd = open(*name, O_WRONLY, 0) == -1)
            fd = creat(*name, PERMS);
        lseek(*name, 0L, 2);
    } else 
        fd = open(*name, O_RDONLY, 0);
    if (fd == -1)      //上面的补充如果文件打开失败
        return NULL;
    fp->_base = NULL;
    fp->_file = fd;
    fp->_cnt = 0;
    fp->_flag = (*mode == 'r') ? _IOREAD : _IOWRT;
    return fp;
}

int flushbuf()