#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"rpn.h"

#ifndef MAXVAL
    #define MAXVAL 100
#endif


//获取下一个运算符
int getop(char s[])
{
    int i, c;

    while ((c = getch()) == ' ' || c == '\t')
        s[0] = c;
    s[1] = '\0';
    if (!isdigit(c) && c != '.')
        return c;       //不是数
    i = 0;
    if (isdigit(c))
        while (isdigit(c = getch()))
            s[++i] = c;     //收集整数部分
    if (c == '.')
        while (isdigit(c = getch()))
            s[++i] = c;     //收集小数部分
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

int sp = 0;
double val[MAXVAL];

//将计算的中间值压入值栈
void push(double d)
{
    if (sp <= MAXVAL)
        val[sp++] = d;
    else
        printf("errpr: stack full, can't push %g\n", d);
}

//从栈中弹出并返回栈顶值
double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0;
    }
}



