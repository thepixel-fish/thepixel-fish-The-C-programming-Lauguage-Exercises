//bit_fopen & bit_fillbuf用位字段替代位操作
//位字段
//_fillbuf填充缓冲区不起作用    error: 表达式必须具有结构或联合类型，但它具有类型 "int"
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#define PERMS 0666

FILE *my_fopen(char *name, char *mode);
int fillbuf(FILE *);

struct {
    unsigned int is_read    : 1;
    unsigned int is_write   : 1;
    unsigned int is_unbuf   : 1;
    unsigned int is_buf     : 1;
    unsigned int is_eof     : 1;
    unsigned int is_err     : 1;
} flag_filed;

int main(int argc, char *argv[])
{
    FILE *fp;
    while (--argc > 0){
        fp = my_fopen(*++argv, "r");
        fillbuf(fp);
    }
    printf("%s", fp->_ptr);
    return 0;
}

int  fillbuf(FILE *fp)
{
    int bufsize;

    if ((fp->_flag & (_IOREAD | _IOEOF | _IOERR)) != _IOREAD)
        return EOF;
    bufsize = (fp->_flag & _IONBF) ? 1 : BUFSIZ;
    if (fp->_base == NULL)
        if ((fp->_base = (char *) malloc(bufsize)) == NULL)
            return EOF;
    fp->_ptr = fp->_base;
    fp->_cnt = read(fp->_file, fp->_ptr, bufsize);
    fp->_flag.is_read = 0;
    if (--fp->_cnt < 0){
        if (fp->_cnt == -1)
            fp->_flag |= _IOEOF;
        else
            fp->_flag |= _IOERR;
        fp->_cnt = 0;
        return EOF;
        }
    return (unsigned char) *fp->_ptr++;
}

FILE *my_fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    fp->_flag.is_read = 0;
    fp->
    if (*mode != 'r' && *mode != 'w' && *mode && 'a')
        return NULL;
    for (fp = _iob; fp < _iob + FOPEN_MAX; fp++)
        if (fp->_flag.is_read == 0 && fp->flag.is_write == 0)    //找到无读写权的位置->>即空位
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
    if (*mode == 'r'){
        fp->_flag.is_read = 1;
        fp->_flag.is_write = 0;
    } else {
        fp->_flag.is_read = 0;
        fp->_flag.is_write = 1;
    }
    return fp;
}