#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define HASHSIZE 101    //e.g. 名字为HASHSIZE，替换文本为101的def行为
#define MAXWORD 100

typedef struct nlist{
    char *name;
    char *defn;
    struct nlist *next;
}nlist;

static nlist *hashtab[HASHSIZE];

nlist *getdef(void);                 //获取name和defn
nlist *install(char *, char *);     //替换或添加新def并接入散列表
nlist *lookup(char *);              //在散列表中查询定义
unsigned hash(char *);              //获取散列值
void undef(char * );                //取消定义
void listprint(nlist **, int);       //test
static char End[4] = "end";         //test

int getword(char *, int);
int getch(void);
void ungetch(int);
char *my_strdup(const char *);
nlist *nalloc(void);

int main(int argc, char *argv[])
{
    char word[MAXWORD];
    nlist *p;
    static char *defi = "define";
    static char *undefi = "undef";

    while (getword(word, MAXWORD) != EOF && strcmp(word, End)){
        if (*word == '#'){
            getword(word, MAXWORD);
            if (strcmp(word, defi) == 0){
                p = getdef();
                install(p->name, p->defn);
            } else if (strcmp(word, undefi) == 0){
                p = getdef();
                undef(p->name);
            }
        }
    }
    listprint(hashtab, HASHSIZE);
    return 0;
}

void listprint(nlist **s, int lim)
{   
    int i;

    for  (i = 0; i < lim; ++i)
        while (s[i] != NULL){
            printf("%s  %s", s[i]->name, s[i]->defn);
            s[i] = s[i]->next;
        }
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
getdef(void)
{
    char w[MAXWORD];
    nlist *p;
    char c;
    int i = 0;

    p = nalloc();
    p->next = NULL;
    while ((c = getch()) == '\40')
        ;
    for (;(isalnum(c) || c == '_') && c != '\40'; c = getch())
        w[i++] = c;
    w[i] = '\0';
    p->name = my_strdup(w);
    if (c == '\n'){
        p->defn = NULL;
        return p;
    }
    for(i = 0; isalnum(c = getch()) || c == '_';)
        w[i++] = c;
    ungetch(c);
    w[i] = '\0';
    p->defn = my_strdup(w);
    return p;
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

int getword(char *w, int maxword) {
	char c;
	int i;
	while ((c = getch()) == '\40' || c == '\t')
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c) && c != '_' && c!= '"' && c != '\'' && c!='/' ) {
		*w = '\0';
		return c;       //“#”会直接返回
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
