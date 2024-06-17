//读入C文件，变量名分组
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>


#define MAXWORD 100
#define YES 1
#define NO 0

int comp = 6;   //比对位数
typedef struct tnode{
    char *word;
	int match;
    struct tnode *left;
    struct tnode *right;
}tnode;

int comment(void);
int getword(char *, int);
int getch(void);
void ungetch(int c);

tnode *addtree(tnode*, char *, int, int *);
tnode *talloc(void);
int compare(tnode *, char *, int, int *);
char *my_strdup(const char *s);
void treeprint(tnode *);



int main(int argc, char *argv[])
{
    tnode *root;
    char word[MAXWORD];
	int found = NO;

    root = NULL;
    while(getword(word, MAXWORD) != EOF){
        if (isalpha(word[0]) && strlen(word) > comp)
        	root = addtree(root, word, comp, &found);
		found = NO;
    }
    treeprint(root);
    return 0;
}

tnode *addtree(tnode *p, char *w, int num, int *found)
{
    int cond;

    if (p == NULL){
        p = talloc();
		p->match = NO;
        p->word = my_strdup(w);    //为单词确定空间大小并申请p->word的空间后，复制到p->word的空间
        p->left = p->right = NULL;
    } else if (cond = compare(p, w, num, found) == 0)
    	;            //新词与该节点匹配
    else if (cond < 0)          //小于该节点，进入左子树
        p->left = addtree(p->left, w, num, found);        //error:要将新节点保存到父节点中
    else                        //大于该节点，进入右子树
        p->right = addtree(p->right, w, num, found);
    return p;
}
int getword(char *word, int max)
{
	int c, d, comment(void);
	char *w = word;
	
	while (isspace(c=getchar()))
		; //跳过空格
	if (c != EOF)
		*w++ = c;
	if (isalpha(c) || c == '_' || c == '#'){
		for (; --max > 0; w++)
			if (!isalnum(*w = getchar()) && *w != '_'){
				ungetc(*w, stdout); 
				break;
			}
	} else if (c == '\'' || c == '"'){
			for (;--max > 0; w++)
				if ((*w = getchar()) == '\\')
					*(++w) = getchar();//跳过\\视为单词的一部分
				else if(*w == c){//如果等于右双引号或单引号，也就是等于c
					w++;
					break;
			} else if (*w == EOF){
					break;
			}
	} else if (c == '/'){
		if ((d = getchar()) == '*')
			c = comment();
		else 
			ungetc(d,stdout);
	}
		*w = '\0'; //非单词防止错误填入
		return c;
}

#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;
int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp < BUFSIZE)
		buf[bufp++] = c;
	else
		printf("error: the buf is full\n");
}

tnode *
talloc(void)
{
    return (tnode *) malloc(sizeof(tnode)); //申请一个tnode大小的空间并以tnode*的型式返回
}

//获取注释内的内容
int comment(void)
{
	int c;

	while((c = getchar()) != EOF){
		if (c == '*'){
			if ((c = getchar()) == '/')
				break;
			else
				ungetc(c,stdout);
		}
	}
	return c;
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

int compare(tnode *p, char *w, int num, int *found)
{
	int i;
	char *t = p->word;
	char *s = w;
	for (i = 0; *s == *t; ++i, ++s, ++t)
	if (*s == '\0')
		return 0;

	if (i >= num){
		p->match = YES;
		*found = YES;
	}
	return *s - *t;
}

//中序遍历打印树
void treeprint(tnode *p) 
{
	treeprint(p->left);
	printf("%s\t", p->word);
	treeprint(p->right);
}
