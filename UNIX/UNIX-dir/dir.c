#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>

#define MAX_PATH 1024

void dirwalk(char *, void (*fcn)(char *));
void fsize(char *name);
DIR *my_opendir(char *dirname);     //DIR中文件描述符不对，不能使用
void my_closedir(DIR *);
int main(int argc, char **argv);
struct dirent *my_readdir(DIR *);

int main(int argc, char **argv)
{
    if (argc == 1)
        fsize(".");
    else
        while (--argc > 0)
            fsize(*++argv);
    return 0;
}

void fsize(char *name)
{
    struct stat stbuf;          //保存name对应文件的ino
    if (stat(name, &stbuf) == -1){
        fprintf(stderr, "fsize: can't open %s", name);
        return;                 //void的可直接return.不返回任何值
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name, fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
}

void dirwalk(char *dir, void (*fcn)(char *))
{
    DIR *dfd;
    struct dirent *dp;
    char name[MAX_PATH];

    if ((dfd = opendir(dir)) != NULL){
        fprintf(stderr, "dirwalk: %s open fail\n", dir);
        return;
    }
    while ((dp = readdir(dfd)) != NULL){
        printf(dp->d_name);     //*调试查看语句
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;           //跳过自身以及父目录，否则死循环
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name)){       //查看路径是否超出限度
            fprintf(stderr, "dirwalk: name too long\n%s%s", dir, name);
            return;
        } else {
            sprintf(name,"%s%s", dir, name);        //此处给name赋值，将路径赋给name
            (*fcn)(name);                           //执行递归fsize
        }
    }
    closedir(dfd);
}

DIR *my_opendir(char *dirname)
{
    int fd;
    struct stat stbuf;
    DIR *dp;

    if ((fd = open(dirname, O_RDONLY, 0)) == -1 
    || fstat(fd, &stbuf) == -1
    || ((stbuf.st_mode & S_IFMT) != S_IFDIR)
    || (dp = (DIR *)malloc(sizeof(DIR))) == NULL)
        return NULL;
     dp->dd_handle = fd;            //将dir中指向文件描述符。
    return dp;
}

void my_closedir(DIR *dp)
{
    if (dp){
        close(dp->dd_handle);       //关闭文件
        free(dp);
    }
}

struct dirent *my_readdir(DIR *dfd)
{
    struct dirent dirbuf;
    static struct dirent d;

    while (read(dfd->dd_handle, &dirbuf, sizeof(dirbuf)) > 0){
        if (dirbuf.d_ino == 0)
            continue;
        d.d_ino = dirbuf.d_ino;
        strncpy(d.d_name, dirbuf.d_name, _MAX_DIR);
            d.d_name[_MAX_DIR] = '\0';
            return &d;
    }
    return NULL;
}

