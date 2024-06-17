//练习6.4 编写一个程序，根据单词的出现频率按降序打印输入的各个不同的单词，并在每个单词的前面标上它的出现次数
//达成！！

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAXWORD 100

typedef struct lnode{
    char *word;
    int fnum;
    struct lnode *next;
    struct lnode *prior;
}lnode;

char *my_strdup(const char *);
int getword(char *, int);
int getch(void);
void ungetch(int c);

lnode *addlink(lnode *, lnode *, char *word);
lnode *lalloc(void);
void InsertSort(lnode *);
void linkprint(lnode *);

int main(int argc, char *argv[])
{
    char word[MAXWORD];
    lnode *head = NULL;
    static char End[4] = "end";

	head = lalloc();
	head->word = NULL;
	head->fnum = 0;
	head->next = NULL;
	head->prior = NULL;

    while (getword(word, MAXWORD) != EOF && strcmp(word, End) != 0)
        head->next = addlink(head->next, head, word);
	InsertSort(head->next);
	linkprint(head->next);
}

//创建双链表
lnode *
addlink(lnode *p, lnode *prior, char *word)
{
	if (p == NULL){
		p = lalloc();
		p->word = my_strdup(word); //不能直接等要把word指的内容复制到新的地址空间
		p->fnum = 1;
		p->next = NULL;
		p->prior = prior;
	}else if (strcmp(word, p->word) == 0)
		p->fnum++;
	else
		p->next = addlink(p->next, p, word);
	return p;
}

//双链表直接插入排序
void InsertSort(lnode *link)
{
	lnode *i, *j, *temp;
	
	for (i = link->next; i != NULL; i = i->next){
		if (i->fnum < i->prior->fnum){
			i->prior->next = i->next;
			if (i->next != NULL)
				i->next->prior = i->prior;
				for (j = i->prior; i->fnum < j->fnum; j = j->prior)
					;  //找到插入点
				i->next = j->next;		//进行插入
				j->next->prior = i;
				i->prior = j;
				j->next = i;
		}
	}
}

//打印双链表
void linkprint(lnode *link)
{
	lnode *p = link;

	if (p != NULL){
		printf("%d, %s   ", p->fnum, p->word);
		linkprint(p->next);
	}
}

lnode *
lalloc(void)
{
	return (lnode *)malloc(sizeof(lnode));
}

int getword(char *w, int maxword) {
	int flag = 0;
	char c;
	int i;
	while ((c = getch()) == '\40' || c == '\t')
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c) && c != '_' && c!='"' && c != '\'' && c!='/' ) {
		*w = '\0';
		return c;
	}
	if (isalpha(c) || c == '_') {
		for (; --maxword > 0 && (isalnum(c = getch()) || c == '_');)
			*w++ = c;
		ungetch(c);
	}
	else if (c == '"' || c=='\'') {
		if (c == '"')
			while (getch() == '\\' || getch() != '"')
				;
		else
			while (getch() != '\'')
				;
	}
	else if (c == '/') {
		if ((c=getch()) == '/')
			for (i = 0; getch() != '\n'; i++)
				;
		else if (c == '*')
			while (getch() != '*' || getch() != '/')
				;
	}
	*w = '\0';
	return w[0];
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

char *
my_strdup(const char *s)
{
    char *p;

    p = (char *) malloc(strlen(s) + 1); //申请一个新空间
    if (p != NULL)  //防错，比如空间已满
    	strcpy(p, s);   //将旧位置的s复制到p的新空间
    return p;   //返回新空间指针
}

