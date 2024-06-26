#include<stdio.h>
#include<stdarg.h>
#include<ctype.h>

#define LOCALFMT 100

//使用变量(localfmt)保存整体格式(%2.3d之类)传入printf(localfmt, ...)

void miniprintf(char *fmt, ...);

int	main(void)
{
    int j = 1;
    double flo = 1500.2;

    miniprintf("%02d\n", j);
    miniprintf("%0.2f", flo);
    return 0;
}

//添加实现其他%e，%u，%o等功能。以及wide与精确度等功能
void miniprintf(char *fmt, ...)
{
    char localfmt[LOCALFMT];
    char *p, *sval;
    va_list ap;
    int i, ival;
    double dval;
    unsigned uval;

    va_start(ap, fmt);
    for (p = fmt; *p; p++){
        if (*p != '%'){
            putchar(*p);
            continue;
        }
        i = 0;
        localfmt[i++] = '%';
        while (*(p + 1) && !isalpha(*(p + 1)))      //isdigit()检查字符串是否由数字组成
            localfmt[i++] = *++p;
        localfmt[i++] = *(p + 1);
        localfmt[i] = '\0';
        switch(*++p){
        case 'd':
        case 'i':
        case 'c':      //调试检测能否处理%c
            ival = va_arg(ap, int);
            printf(localfmt, ival);
            break;
        case 'o':
        case 'x':
        case 'X':
        case 'u':
            uval = va_arg(ap, unsigned);
            printf(localfmt, uval);
            break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            dval = va_arg(ap, double);
            printf(localfmt, dval);
            break;
        case 's':
            sval = va_arg(ap, char *);          //for (sval = va_arg(ap, char *); *sval != NULL; sval++)
            printf(localfmt, sval);             //替换成以上localfmt方式应该也行
            break;
        default:
            printf(localfmt);
            break;
        }
    }
    va_end(ap);
}