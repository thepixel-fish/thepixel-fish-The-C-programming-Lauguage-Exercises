#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAXWORD 100

int getword(char *, int);
int comment(void);

int main(int argc, char *argv[])
{
    char word[MAXWORD];
    int i;

    while (getword(word, MAXWORD) != EOF){
        for (i = 0; word[i] != '\0'; ++i)
            putchar(word[i]);
    }
    return 0;
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