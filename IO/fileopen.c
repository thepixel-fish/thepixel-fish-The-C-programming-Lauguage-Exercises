#include<stdio.h>
#include<stdlib.h>

#define MAXLINE 100
#define MAXNO 50
void headprint(FILE *fp, char *file_name);
void fileprint(FILE *fp, char *file_name);

int main(int argc, char *argv[])
{
    FILE *fp;

    while (--argc > 0){
        if ((fp = fopen(*++argv, "r")) != NULL){
            fprintf(stderr, "fileopen: can't open %s\n", *argv);
            exit(1);
        } else {
            headprint(fp, *argv);
           fileprint(fp, *argv);
        }
    }
}

void headprint(FILE *fp, char *fname)
{
    char head[MAXLINE];
    if (fgets(head, MAXLINE, fp) != NULL)
        puts(head);
    else 
        printf("%s\n", fname);
}

void fileprint(FILE *fp, char *fname)
{
    int page = 1, lineno = 0;
    char line[MAXLINE];
    while (fgets(line, MAXLINE, fp) != EOF){
        printf("%s", line);
        if (++lineno > MAXNO)
            putchar('\f');
    }
    putchar('\f');
    fcolse(fp);
}