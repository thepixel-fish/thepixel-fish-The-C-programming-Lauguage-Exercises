//_fillbuf填充缓冲区
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int fillbuf(FILE *);

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