#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define YES 1
#define NO  0
#define MAXLINE 100
int linecomp(FILE*, FILE*);
int main(int argc, char* argv[])
{
    FILE* fp1, * fp2;
    if (argc != 3) {
        fprintf(stderr, "comp: need two file names.\n");
        exit(1);
    }
    else {
        if ((fp1 = fopen(*++argv, "r")) == NULL) {
            fprintf(stderr, "comp: can't open %s.\n", *argv);
            exit(2);
        } else if ((fp2 = fopen(*++argv, "r")) == NULL){
            fprintf(stderr, "comp: can't open %s.\n", *argv);
            exit(2);
        }
        else {
            linecomp(fp1, fp2);
            fclose(fp1);
            fclose(fp2);
            exit(0);
        }
    }

}

int linecomp(FILE* fp1, FILE* fp2)
{
    char line1[MAXLINE], line2[MAXLINE];
    char* lp1, * lp2; //如果fgets到达文件尾会返回NULL，所以当lp与行line相等时，即未到文件尾
    do {
        lp1 = fgets(line1, MAXLINE, fp1);
        lp2 = fgets(line2, MAXLINE, fp2);
        if (lp1 == line1 && lp2 == line2) {
            if (strcmp(line1, line2) != 0) {
                printf("comp: find the defferece in line\n%s\n%s", line1, line2);
                return YES;
            }
        }
        else if (lp1 != line1 || lp2 != line2) {
            fprintf(stderr, "comp: file is end.there're same\n");
            return NO;
        }
    } while (lp1 == line1 && lp2 == line2);
}