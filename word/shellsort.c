//希尔排序

//排序1：从第二个开始，慢慢的加到最后。即第一次处理第一个分表的第一个，然后处理第二个分表第一个，逐个处理直到最后一个。
void shell_sort(int A[], int n) {
    int di;
    int i;
    int j;
    //步长即di的变化
    for (di = n / 2; di >= 1; di = di / 2) {
        //注意，已分好段，每段第二个值开始操作
        for (i = di + 1; i <= n; i++) {
            //检查前面的值是不是比当前的值大，如果比较大的话开始调换
            if (A[i] < A[i - di]) {
                A[0] = A[i];  //暂存在A[0]中
                //一直往后循环，检查后面有没有大于当前值的了（不可越界），如果有，那么进行转换
                for (j = i - di; j > 0 && A[0] < A[j]; j = j - di) {
                    A[j + di] = A[j];
                }
                //最后执行的时候将j = j-di,要回到j = j+di的位置进行插入
                A[j + di] = A[0];
            }
        }
    }
}
//排序2：第二种是从第二个开始，处理第一个分表的第一个，然后处理第一个分表的第二个，直到所有表处理完执行下一轮希尔排序。
//shell变种
//A[0]为暂存单元
void shell_sort2(int A[], int n) {
    int di;
    int i = 0;
    int j ;
    int k;
    //步长即di的变化
    for (di = n / 2; di >= 1; di = di / 2) {
        //注意，已分好段，每段第二个值开始操作
        for (i = di + 1; i < 2 * di + 1; ++i) {
            for (k = i;  k <= n; k = k + di) {
                //检查前面的值是不是比当前的值大，如果比较大的话开始调换
                if (A[k] < A[k - di]) {
                    A[0] = A[i];  //暂存在A[0]中
                    //一直往后循环，检查后面有没有大于当前值的了（不可越界），如果有，那么进行转换
                    for (j = k - di; j > 0 && A[0] < A[j]; j = j - di) {
                        A[j + di] = A[j];
                    }
                    //最后执行的时候将j = j-di,要回到j = j+di的位置进行插入
                    A[j + di] = A[0];
                }
            }
        }
    }
}


int main() {
    int a[] = {-1, 4, 5, 3, 1, 2, 7, 88, 55, 33, 2, 99};
    shell_sort2(a, 11);
    for (int i = 1; i < 12; ++i) {
        printf("%d  ", a[i]);
    }
}
/*————————————————
版权声明：本文为CSDN博主「不会装b的b皇」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/weixin_45860846/article/details/122149279*/