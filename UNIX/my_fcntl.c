//8.3-8.4
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

#define PERMS 0666

FILE *my_fopen(char *name, char *mode);
int fillbuf(FILE *);        //填充读缓存，从fp->fd中读入字符直到buf填充或文件尾
int flushbuf(int x, FILE *fp);  //清空写缓存，将buf中字符输入到fp->fd中
int my_fclose(FILE *);
int my_fseek(FILE *, long offset, int origin);  //在文件中查找位置，file中分读写模式注意缓存内容

//测试用入口
int main(int argc, char *argv[])
{
    FILE *fp;
    while (--argc > 0){
        fp = fopen(*++argv, "r");
        fillbuf(fp);
    }
    printf("%s", fp->_ptr);
    return 0;
}


//_fillbuf填充缓冲区
int  fillbuf(FILE *fp)
{
    int bufsize;

    if ((fp->_flag & (_IOREAD | _IOEOF | _IOERR)) != _IOREAD)   //位运算符进行bool运算？
        return EOF;
    bufsize = (fp->_flag & _IONBF) ? 1 : BUFSIZ;    //注意是否有给文件配备缓冲区
    if (fp->_base == NULL)      //缓冲基地址为空
        if ((fp->_base = (char *) malloc(bufsize)) == NULL)
            return EOF;
    fp->_ptr = fp->_base;       //重置ptr
    fp->_cnt = read(fp->_file, fp->_ptr, bufsize);      //缓冲计数等于读入字符数量
    if (--fp->_cnt < 0){        
        if (fp->_cnt == -1) //读入数量为0，到文件尾
            fp->_flag |= _IOEOF;
        else                //其他负数意味这出错
            fp->_flag |= _IOERR;
        fp->_cnt = 0;
        return EOF;
        }
    return (unsigned char) *fp->_ptr++; //返回缓冲中的第一个字符
}

FILE *my_fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode && 'a')   //无打开模式，参数不对
        return NULL;
    for (fp = _iob; fp < _iob + FOPEN_MAX; fp++) //在iobuf中找空位，
        if ((fp->_flag & (_IOREAD | _IOWRT)) == 0) //找到无读写权的位置->>即空位
            break;
    if (fp >= _iob + FOPEN_MAX)            //如果无空。若没有，该程序打开文件数达到最大值（20一般
        return NULL;
    if (*mode == 'w'){          //写模式，创建文件
        fd = creat(name, PERMS);
    } else if (*mode == 'a'){       //添加模式，打开文件，找到文件尾
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else 
        fd = open(name, O_RDONLY, 0);       //普通打开即可
    if (fd == -1)      //上面的检错如果文件打开失败
        return NULL;
    fp->_base = NULL;      //打开新文件。iobuf+x该位置信息重置
    fp->_file = fd;
    fp->_cnt = 0;
    fp->_flag = (*mode == 'r') ? _IOREAD : _IOWRT;
    return fp;
}
//allocate and flush output buffer.
int flushbuf(int x, FILE *fp)
{
    int bufsize;
    unsigned int nc;

    if (fp < _iob || fp > _iob + BUFSIZ)
        reutrn EOF;                         //指针越界检查
    if ((fp->_flag & (_IOERR | _IOWRT)) != _IOWRT)
        return EOF;
    bufsize = (fp->_flag & _IONBF) ? 1 : BUFSIZ;
    if (fp->_base == NULL){     //缓冲区不存在
        if ((fp->_base = (char *) malloc(sizeof(bufsize))) == NULL){
            fp->_flag |= _IOERR;
            return EOF;
        }
    } else {            //缓冲区存在
        nc = fp->_ptr - fp->_base;      //注意不要直接用bufsize
        if (write(fp->_file, fp->_ptr, nc) != nc){
            fp->_flag |= _IOERR;
            return EOF;
        }
    }
    fp->_ptr = fp->_base;       
    *fp->_ptr++ = (char) x; 
    fp->_cnt = bufsize - 1; //为啥不是0？在写缓冲中，cnt代表可读入字符数？
    return x;
}

//
int my_fflush(FILE *fp)
{
    int ret = 0;
    if (fp < _iob || fp > _iob + FOPEN_MAX)     //越界检查
        return EOF;
    if ((fp->_flag & (_IOERR | _IOWRT)) != _IOWRT)  //是写文件
        ret = flushbuf(0, fp);
    fp->_ptr = fp->_base;
    fp->_cnt = (fp->_flag & _IONBF) ? 1 : BUFSIZ;
    return ret;
}

//保证缓冲区清空
int my_fclose(FILE *fp)
{
    int ret;
    if ((ret = fflush(fp)) != EOF)  {   //ret包含了错误返回
        free(fp->_base);                //Q: 已经free了，为啥要置空？
        fp->_ptr = NULL;                //A: 只需将缓冲区free，剩余的还在_iobuf的数组中
        fp->_cnt = 0;                   //防止_iobuf添加新文件时，文件指针中有无意义的值
        fp->_base = NULL;
        fp->_flag &= ~(_IOWRT | _IOREAD);
    }
    return ret;
}

int my_fseek(FILE *fp, long offset, int origin)
{
    unsigned nc;
    long rc = 0;

    //清除读写缓冲区后再使用lseek
    if (fp->_flag & _IOREAD){       //是否读打开
        if (origin == 1)        //读文件，从当前位置，减去读缓冲字符数
            offset -= fp->_cnt;
        rc = lseek(fp->_file, offset, origin);
    } else if (fp->_flag & _IOWRT){     //写文件，将缓冲中的字符刷新到文件中
        if ((nc = fp->_ptr - fp->_base) > 0)
            if (write(fp->_file, fp->_base, nc) != nc)
                rc = -1;
        if (nc != -1)
            lseek(fp->_file, offset, origin);
    }
    return (rc == -1) ? -1 : 0;
}
