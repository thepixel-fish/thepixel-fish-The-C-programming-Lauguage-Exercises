/*练习7.7 修改第五章的模式查找程序，使它从一个命名文件的集合中读取输入（有文件名参数时）
如果没有文件名参数，则标准输入中获取输入。当发现一个匹配行时，是否应该将相应的文件名打印出来*/
//1.添加标准输入选项else if 2.添加多文件，当本文件有发现时，输出文件名做头（将查找函数拉出来）
//把参数调调位置代码能简单不少…理解为啥有的命令那么输入起来那么奇怪了…
//done！
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//-n 打印行号 -x 打印除找到行外
#define MAXLINE 100
char *my_strdup(const char *);
void fpat(FILE *fp, char *filename, char *parttern, int except, int number);

int main(int argc, char *argv[])
{
    FILE *fp;
    char *parttern;
    int c, number = 0, except = 0;

    //parttern = my_strdup(*(argv + argc - 1)); 是否有出查找样式不确定,文件名在样式之后
    while(--argc > 0 && *(++argv)[0] == '-')    //控制下大括号的数量，尽量减少包含关系，模块化
        while((c = *++argv[0]))              //进一步处理"-xn"或"-nx"(== "-x -n")
            switch(c){
                case 'n':
                    number = 1;
                    break;
                case 'x':
                    except = 1;
                    break;
                default:
                    printf("find: illegal option %c\n", c);
                    argc = 0;
                    break;
            }
    if (argc >= 1)      //判断是否有给出查找样式
        parttern = my_strdup(*argv);
    else {
        printf("Usage: find [-x] [-n] parttern [file...] \n");
        exit(1);
    }
    if (argc == 1)
        fpat(stdin, "", parttern, except, number);
    else
        while(--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL)
                fprintf(stderr, "find: can't open %s\n", *argv);
            else {
                fpat(fp, *argv, parttern, except, number);
                fclose(fp);
            }
    return 0;
}

char *my_strdup(const char *s)
{
    char *p;

    p = (char *) malloc(strlen(s) + 1); //申请一个新空间
    if (p != NULL)                      //防错，比如空间已满
    	strcpy(p, s);                   //将旧位置的s复制到p的新空间
    return p;                           //返回新空间指针
}

void fpat(FILE *fp, char *fname, char *parttern, int except, int number)
{
    char line[MAXLINE];
    long lineno = 0;

    while (fgets(line, MAXLINE, fp) != NULL){
        lineno++;
        if((strstr(line, parttern) != NULL) != except){
            if(*fname)
                printf("%s-", fname);
            if (number)
                printf("%ld ", lineno);
            puts(line);
        }
    }
}

