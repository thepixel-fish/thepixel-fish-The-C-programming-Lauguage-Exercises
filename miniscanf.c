#include<stdio.h>
#include<stdarg.h>
#include<ctype.h>

#define LOCALFMT 100
void miniscanf(char *, ...);

int main(void)
{
	int i;
	char s[100];
	float f = 0;
	double d;
	
//	miniscanf("int%d", &i);
//	scanf("string%s", s);
	scanf("%f", &f);
//	miniscanf("double%lf", &d);
	
	printf("%f", f);
	return 0;
}

void miniscanf(char *fmt, ...)
{
    va_list ap;
    char localfmt[LOCALFMT];
    char *p, *sval;
    int i, flag, *ival;
    double *dval;
    float *fval;
    unsigned *uval;

	i = 0;
    va_start(ap, fmt);
    for (p = fmt; *p; p++){
        if (*p != '%'){
            localfmt[i++] = *p;
            continue;
        }
        localfmt[i++] = '%';
        while (*(p + 1) && !isalpha(*(p + 1)))
            localfmt[i++] = *++p;
        localfmt[i++] = *(p + 1);
        localfmt[i] = '\0';
        switch(*++p){
        case 'd':
        case 'i':
        case 'o':
        case 'x':
            ival = va_arg(ap, int *);
            scanf(localfmt, ival);
            break;
        case 'u':
            uval = va_arg(ap, unsigned *);
            scanf(localfmt, uval);
            break;
        case 'c':
        case 's':
            sval = va_arg(ap, char *);
            scanf(localfmt, sval);
            break;
        case 'e':
        case 'f':
        case 'g':
            if (flag == 1){
                dval = va_arg(ap, double *);
                scanf(localfmt, dval);
            }
            else{
                fval = va_arg(ap, float *);
                scanf(localfmt, fval);
            }
            flag = 0;
            break;
        }
    }
    va_end(ap);
}