#include<dirent.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>

#define MAX_PATH  1024

void dirwalk(char *dir, void (*fcn)(char *));
void fsize(char *name);

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
    if (stat(name, &stbuf) == -1){      //stat fail始终返回-1
        fprintf(stderr, "fsize: can't open %s", name);
        return;                 //void的可直接return.不返回任何值
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name, fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
    printf("%8lld %8lld", stbuf.st_atime,stbuf.st_ctime);
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
