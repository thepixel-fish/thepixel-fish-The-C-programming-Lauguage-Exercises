/*解题思路：先利用二叉树结构对输入变量名进行排序，然后将二叉树转换为双向
链表，对双向链表的每个结点分组，最后按照组号把链表打印出来*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

enum {NO,YES};

struct node {
	char *word;
	struct node *left;
	struct node *right;
	int group;
};

#define MAXWORD 100
char word[MAXWORD];
int getword(char *w, int maxword);
int getch(void);
void ungetch(int c);
struct node *addtree(struct node *p, char *t,int num);
int compare(struct node *p, char *t,int num);
void Listconv(struct node *p,struct node **lastnode);
struct node *convert(struct node *root);
void groupdiv(struct node *p,int num);
void treeprint(struct node *listhead);
struct node *talloc(void);
char *my_strdup(char *s);

int main(int argc, char *argv[]) {
	struct node *root=NULL;
	struct node *listhead = NULL;
	int num,i;
	char n[100],c;
	if (argc == 1 || (*++argv)[0] != '-')
		num = 6;
	else {
		for (i = 0; isdigit(c=*++argv[0]); i++)
			n[i]= c;
		n[i] = '\0';
		num = (atoi(n) > 0)? atoi(n):6;
	}
	while (getword(word, MAXWORD) != EOF) {
		if (isalpha(word[0]))
			root = addtree(root,word,num);//构建二叉树
	}
	listhead = convert(root);		//将二叉树转换为双向链表，并返回链表表头
	groupdiv(listhead, num);		//对链表中每个结点分组
	treeprint(listhead);		   //按照组号打印链表
	return 0;
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



//构建二叉树
struct node *addtree(struct node *p, char *t,int num) {
	int cond;
	if (p == NULL) {
		p = talloc();
		p->word = my_strdup(t);
		p->left = NULL;
		p->right = NULL;
	}
	else if ((cond = compare(p, t,num)) < 0)
		p->left = addtree(p->left, t, num);
	else if (cond > 0)
		p->right = addtree(p->right, t,num);
	return p;
}

int compare(struct node *p, char *t,int num) {
	int i;
	char *s = p->word;
	for (i = 0; *s == *t; i++,s++,t++)
		if (*s == '\0')
			return 0;
	return *t - *s;
}

//构造双向链表
void Listconv(struct node *p,struct node **lastnode) {
	struct node *current = p;
	if (current == NULL)
		return;
	if (current->left != NULL)
		Listconv(current->left, lastnode);
	current->left = *lastnode;
	if(*lastnode!=NULL)
		(*lastnode)->right = current;
	*lastnode = current;
	if (current->right != NULL)
		Listconv(current->right, lastnode);
}

struct node *convert(struct node *root) {
	struct node *lastnode = NULL;
	struct node *listhead;
	Listconv(root, &lastnode);
	listhead = lastnode;
	//找到链表表头
	while (listhead != NULL && listhead->left != NULL)
		listhead = listhead->left;
	return listhead;
}
//对链表内数据进行分组
void groupdiv(struct node *p,int num) {
	int i;
	char *word, *word1;
	p->group = 0;
	while (p != NULL && p->right!=NULL) {
		word = p->word;
		word1 = (p->right)->word;
		for (i = 0; *word == *word1 && *word!='\0'; word++, word1++, i++)
			;
		if (i >= num)
			p->right->group = p->group;
		else
			p->right->group = p->group + 1;
		p = p->right;
	}
}
//按照组号打印链表，组与组之间用换行符隔开
void treeprint(struct node *p) {
	if (p != NULL) {
		printf("%s\n", p->word);
	}
	while (p != NULL && p->right!=NULL) {
		if (p->right->group == p->group)
			printf("%s\n", p->right->word);
		else
			printf("\n%s\n", p->right->word);
		p=p->right;
	}
}


struct node *talloc(void) {
	return (struct node *)malloc(sizeof(struct node));
}

char *my_strdup(char *s) {
	char *p;
	p = (char *)malloc(strlen(s) + 1);
	if (p != NULL)
		strcpy(p, s);
	return p;
}

/*版权声明：本文为CSDN博主「dream_fun」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/dream_fun/article/details/114171928*/