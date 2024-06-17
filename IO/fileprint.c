#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int page = 0;

    while (--argc > 0){
        if ((fp = fopen(*++argv, "r")) != NULL)
            fprintf()
    }
}