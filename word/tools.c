


tnode *addtree(tnode *p, char *w)
{
    int cond;

    if (p == NULL){
        p = talloc();
        p->word = strdup(w);    //为单词确定空间大小并申请p->word的空间后，复制到p->word的空间
        p->left = p->right = NULL;
    } else if (cond = compare(p, w, comp) == 0)
       break;            //新词与该节点匹配
    else if (cond < 0)          //小于该节点，进入左子树
        p->left = addtree(p->left, w, comp);        //error:要将新节点保存到父节点中
    else                        //大于该节点，进入右子树
        p->right = addtree(p->right, w, comp);
    return p;
}
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

tnode *
talloc(void)
{
    return (tnode *) malloc(sizeof(tnode)); //申请一个tnode大小的空间并以tnode*的型式返回
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

char *
my_strdup(const char *s)
{
    char *p;

    p = (char *) malloc(strlen(s) + 1); //申请一个新空间
    if (p != NULL)  //防错，比如空间已满
    strcpy(p, s);   //将旧位置的s复制到p的新空间
    return p;   //返回新空间指针
}