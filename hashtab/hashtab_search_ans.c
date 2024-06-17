//表现更为完善的define预处理器，但无法运行。
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define HASHSIZE 101    //e.g. 名字为HASHSIZE，替换文本为101的def行为
#define MAXWORD 100

typedef struct nlist{
    char *name;
    char *defn;
    struct nlist *next;
}nlist;

nlist *hashtab[HASHSIZE];

void skipblanks(void);
void error(int, char *);
void getdef(void);
void undef(char *);
nlist *install(char *, char *);     //替换或添加新def并接入散列表
nlist *lookup(char *);              //在散列表中查询定义
unsigned hash(char *);

char *my_strdup(const char *);
nlist *nalloc(void);
char getword(char *, int);
int getch(void);
void ungetch(int);
void ungets(char *); //将整段字符串压回输入

void skipblanks(void)
{
	int c;
	while ((c = getch()) == ' ' || c == '\t')
		;
	ungetch(c);
}

int main(int argc, char *argv[])
{
	char word[MAXWORD];
    nlist *p;

    while (getword(word, MAXWORD) != EOF){
        if (strcmp(word, "#") == 0)
            getdef();
        else if (!isalpha(word[0]))
            printf("%s", word);
        else if ((p = lookup(word)) == NULL)
            printf("%s", word);
        else
            ungets(p->defn);
    }
}
void getdef(void)
{
	int i;
	char def[MAXWORD], dir[MAXWORD], name[MAXWORD];

	skipblanks();
	if (!isalpha(getword(dir, MAXWORD)))		//检查语法合法性
		error(dir[0], "getdef: expecting a directive after #");
	else if (strcmp(dir, "define") == 0){		//如果是define定义
		skipblanks();
		if (!isalpha(getword(name, MAXWORD)))	//获取name并检查合法性
			error(name[0], "getdef: expecting a directive after #");
		else {
			skipblanks();
			for (i = 0; i < MAXWORD - 1; ++i)
				if ((def[i] = getch()) == EOF || def[i] == '\n') //可定义但无定义时
					break;
			def[i] = '\0';
			if (i <= 0)						//无defn，不完整的定义
				error('\n', "getdef: non-alpha - name expected");
			else
				install(name, def);
		}
	} else if (strcmp(dir, "undef") == 0){
		skipblanks();
		if (!isalpha(getword(name, MAXWORD)))	
			error(name[0], "getdef: non-alpha in undef");
		else
			undef(name);
	} else
		error(dir[0], "getdef: expecting a directive after #");
}

//打印错误信息，并跳过剩余行(只get不输出)
void error(int c, char *s)
{
	printf("error: %s\n", s);
	while(c != EOF && c !='\n')
		c = getch();
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

//将整个字符串压回输入
void ungets(char s[])
{
	int i;
	for (i = 0; s[i] != '\0'; i++)
		;
	for(; i >= 0; i--)
	{
		ungetch(s[i]);
	}
}

char getword(char *w, int maxword) {
	char c;
	int i;
	char *s;

	s = w;
	while ((c = getch()) == '\t')
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c) && c != '_' && c!= '"' && c != '\'' && c!='/' ) {
		*w = '\0';
		return c;       //“#”,“ ”会直接返回
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
	return s[0];
}

unsigned hash(char *s)
{
    unsigned int hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;  //%HASHSIZE，保证hashtab可以装的下这些关键字
}

nlist *
lookup(char *name)
{
    nlist *p;

    for (p = hashtab[hash(name)];  p != NULL; p = p->next)
        if (strcmp(name, p->name) == 0)
            return p;
    return NULL;
}

nlist *
install(char *name, char *defn)
{
    nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL){
        np = nalloc();
        if (np == NULL || (np->name = my_strdup(name)) == NULL)
            return NULL;                        //内存已满无法申请空间时，防错
        hashval = hash(name);                  //找到key的位置
        np->next = hashtab[hashval];           //将新节点插入该链表中（散列邻接表）
        hashtab[hashval] = np;
    } else 
        free((void *)np->defn);
    if ((np->defn = my_strdup(defn)) == NULL)     //传入defn，新建的节点也要存defn所以该语句在if else外。
            return NULL;                      //内存已满无法申请空间时，放错。
    return np;
}

nlist *
nalloc(void)
{
    return (nlist *)malloc(sizeof(nlist));
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

void undef(char *s)
{
    nlist *p;
    nlist *prev = NULL;
    int hashval;

    hashval = hash(s);
    for (p = hashtab[hashval]; p != NULL; p = p->next){
        if ((strcmp(s, p->name)) == 0)
            break;
        prev = p;
    }
    if (p != NULL) {      //有找到
        if (prev == NULL)    //不是邻接第一个
            hashtab[hashval] = p->next;
        else
            prev->next = p->next;
        free((void *) p->name);
        free((void *) p->defn);
        free((void *) p);
    }
}
