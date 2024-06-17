//位运算符的运用
#include<stdio.h>

int main(void)
{
    int x = 24;
    printf("%d", x&1);      //e.g. (x&1 == 1) ->> 奇数，（x&1 == 0) ->>偶数
    return 0;
}

//获取x中第p位开始的n位
unsigned getbits(int x, int p, int n)
{
    return (x >> (p + 1 -n)) & ~(~0 << n);            //假定最右位为第0位
}