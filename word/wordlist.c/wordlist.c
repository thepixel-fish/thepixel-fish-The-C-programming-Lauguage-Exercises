#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include"word.h"

#define MAXWORD 100

typedef struct line{
    int line_num;
    struct line *next;
}line;

typedef struct lnode{
    char *word;
    line *num;
    struct lnode *next;
}lnode;

int getch(void);
void ungetch(int);
int line_getword(char *, int, int *);
lnode *lalloc(void);
line *nalloc(void);
int wordcmp(char *);
lnode *addlink(lnode *, char *, int);
void addline(lnode *, int);
void lineprint(lnode *);

int main(int argc, char *argv[])
{
    int line_num = 1;		//行号
    char word[MAXWORD];
	lnode *head = NULL;

    while (line_getword(word, MAXWORD, &line_num) != EOF)
        if (wordcmp(word))
			head = addlink(head, word, line_num);
	lineprint(head);
	return 0;
}

void lineprint(lnode *p)
{
    printf("%s", p->word);
    while (p->num != NULL){
        printf(" %3d", p->num->line_num); 
        p->num = p->num->next;
    }
    lineprint(p->next);
}

lnode *
addlink(lnode *p, char *word, int num)
{
    int cond = 0;

	if (p == NULL){
		p = lalloc();
        p->word = word;
        p->num = nalloc();
        p->num->line_num = num;
        p->num->next = NULL;
        p->next = NULL;
    }
    if ((cond = strcmp(word, p->word)) == 0){
        while(p->num->next != NULL && p->num->line_num != num)
            p->num = p->num->next;  //遍历至最后一个节点
        p->num = nalloc();
        p->num->line_num = num;
        p->num->next = NULL;
    } else 
        p->next = addlink(p->next, word, num);
    return p;
}

int wordcmp(char *word)
{
    static char *and = "and";
    static char *of = "of";

    if (strcmp(word, and) == 0)
		return 0;
	else
		return 1;
}

lnode *lalloc(void)
{
    return (lnode *)  malloc(sizeof(lnode));
}

line *nalloc(void)
{
    return (line *) malloc(sizeof(line));
}

#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;
int getch(void) {
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
	if (bufp < BUFSIZE)
		buf[bufp++] = c;
	else
		printf("error: the buf is full\n");
}

int line_getword(char *w, int maxword, int *line) {
	int flag = 0;
	char c;
	int i;
	while ((c = getch()) == '\40' || c == '\t')
		;
    if (c == '\n')
        *line++;
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