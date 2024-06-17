#include<string.h>
#include<stdio.h>

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