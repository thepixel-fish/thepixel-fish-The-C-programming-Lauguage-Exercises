#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
//统计关键字的出现次数
#define NKEYS 100
#define MAXWORD 100

typedef struct ckey{
		char *word;
		int count;
}key;

key keytab[NKEYS] = {	
	{"auto", 0},
	{"break", 0},
	{"case", 0},
	{"char", 0},
	{"const", 0},
	{"continue", 0,},
	{"default", 0,},
	{"unsigned", 0,},
	{"void", 0},
	{"volatile", 0},
	{"while", 0},
};

int getword(char *, int);
int binsearch(char *, key *, int);
int comment(void);

int main()
{
	int n;
	char word[MAXWORD];
	
	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			if((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
	system("pause");
	for(n = 0; n < NKEYS; n++)
		printf("%4d, %s\n",
				keytab[n].count, keytab[n].word);
	return 0; 
}

//从输入中获取单词
int getword(char *word, int max)
{
	int c, d, comment(void), getch(void);
	void ungetch(int);
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

//获取注释内的内容
int comment(void)
{
	int c;
	void ungetch(int);
	int getch(void);
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

//折半查找
int binsearch(char *word, key *keytab, int n){
	int cond;
	int high, low, mid;
	
	low = 0;
	high = n - 1;
	while (low <= high){
		mid = (high + low) / 2;
		if ((cond = strcmp(word, keytab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}