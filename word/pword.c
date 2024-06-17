//自引用结构：二叉树递归
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include"conio.h"

#define MAXWORD 100
typedef struct tnode{
    char *word;
    int count;
    struct tnode *left;//左子树：字典序小
    struct tnode *right;//右子树：字典序大
}tnode;

int getword(char *, int);

tnode *talloc(void);    //为新节点申请空间
char *my_strdup(const char *);   //复制字符串到某个位置,const防止传入值被修改
tnode *addtree(tnode *, char *);
void treeprint(tnode *);
int comment(void);

/*  word frequency count */
int main(int argc, char *argv[])
{
    tnode *root;
    char word[MAXWORD];

    root = NULL;    //初始化指针
    while((getword(word, MAXWORD)) != EOF)
        if(isalpha(word[0]))
            root  = addtree(root, word);
    treeprint(root);
}

//从输入中获取单词
int getword(char *word, int max)
{
	int c, d, comment(void), getch(void);
	char *w = word;
	
	while (isspace(c=getch()))
		; //跳过空格
	if (c != EOF)
		*w++ = c;
	if (isalpha(c) || c == '_' || c == '#'){
		for (; --max > 0; w++)
			if (!isalnum(*w = getch()) && *w != '_'){
				ungetch(*w); 
				break;
			}
	} else if (c == '\'' || c == '"'){
			for (;--max > 0; w++)
				if ((*w = getch()) == '\\')
					*(++w) = getch();//跳过\\视为单词的一部分
				else if(*w == c){//如果等于右双引号或单引号，也就是等于c
					w++;
					break;
			} else if (*w == EOF){
					break;
			}
	} else if (c == '/'){
		if ((d = getch()) == '*')
			c = comment();
		else 
			ungetch(d);
	}
		*w = '\0'; //非单词防止错误填入
		return c;
}

//打印树中单词与count
void treeprint(tnode *p)
{
    if (p != NULL){                 //前序遍历打印,先到达最左边
        treeprint(p->left);     //左
        printf("%4d, %s\n", p->count, p->word); //中
        treeprint(p->right);    //右
    }
        
}

/* addtree: add a node with w, at or below p */
//添加树，在p的位置或者p的下方增加一个新节点 
tnode *
addtree(tnode *p, char *w)
{
    int cond;

    if (p == NULL){
        p = talloc();
        p->word = strdup(w);    //为单词确定空间大小并申请p->word的空间后，复制到p->word的空间
        p->count = 1;   //初始化
        p->left = p->right = NULL;
    } else if (cond = strcmp(p->word, w) == 0)
        p->count++;             //新词与该节点匹配
    else if (cond < 0)          //小于该节点，进入左子树
        p->left = addtree(p->left, w);        //error:要将新节点保存到父节点中
    else                        //大于该节点，进入右子树
        p->right = addtree(p->right, w);
    return p;
}

tnode *
talloc(void)
{
    return (tnode *) malloc(sizeof(tnode)); //申请一个tnode大小的空间并以tnode*的型式返回
}

char *
my_strdup(const char *s)
{
    char *p;

    p = (char *) malloc(strlen(s) + 1); //申请一个新空间
    if (p != NULL)  //防错，比如空间已满
    strcpy(p, s);   //将旧位置的s复制到p的新空间
    return p;   //返回新空间指针
}

//获取注释内的内容
int comment(void)
{
	int c;

	while((c = getch()) != EOF){
		if (c == '*'){
			if ((c = getch()) == '/')
				break;
			else
				ungetch(c);
		}
	}
	return c;
}