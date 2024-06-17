#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define NALLOC 1024
void t_malloc(unsigned);
void my_malloc(unsigned);
Header *morecore(unsigned);
void my_free(void *ap);
void t_free(void *ap);
void *calloc1(unsigned n, unsigned size);
void *calloc2(unsigned n, unsigned size);

typedef long Align;
union header {
    struct {
        union header *ptr;
        unsigned size;
    }s;
    Align x;
};
typedef union header Header;

static Header *freep = NULL;
static Header base;
//malloc函数：通用存储分配函数
void my_malloc(unsigned nbytes)
{
    Header *prevp, *p;

    unsigned nunits;

    nunits = ((nbytes + sizeof(Header) - 1) / sizeof(Header) + 1);
    if ((prevp = freep) == NULL){       //首次使用，从空块开始
        base.s.ptr = prevp = freep = &base; //指向自己
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ;prevp = p, p = p->s.ptr){
        if (p->s.size >= nunits){           //足够大
            if (p->s.size == nunits)        //正好
                prevp->s.ptr = p->s.ptr;
            else {   //过大
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;          //更新空闲链表指针
            return (void *)(p + 1); //留出头部空间，指向空闲区域
        }
        if (p == freep)  //到达一圈后，没找到足以容纳的空间
            if ((p = morecore(nunits)) == NULL)       //系统申请
                return NULL;
    }
}

//再打一遍回忆版。。
void t_malloc(unsigned nbytes)
{
    Header *prevp, *p;
    unsigned nunits;

    nunits = ((nbytes + sizeof(Header) - 1) / sizeof(Header) + 1);
    if ((prevp = freep) == NULL)
        base.s.ptr = prevp = freep = &base;
        base.s.size = 0;
    for (p = prevp->s.size; ; prevp = p, p = p->s.ptr){
        if (p->s.size >= nunits){
            if (p->s.size == nunits)
                prevp->s.ptr = p->s.ptr;
        } else {
            p->s.size -= nunits;
            p += p->s.size;
            p->s.size = nunits;
        }
        freep = prevp;
        return (void *)(p + 1);
        if(p == freep)
            if ((p = morecore(nunits)) == NULL)
                return 0;
    }
}

Header *morecore(unsigned nu)
{
    char *cp, *sbrk(int);
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) -1)      //(char *)转换后才能与cp进行比对
        return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    free((void *) (up + 1));
    return freep;
}

void my_free(void *ap)
{
    Header *bp, *p;

    bp = (Header *) (ap - 1);
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))     //why.(bp > p || bp < p->s.ptr)
            break;      //why break.也找到位置了。
    if (bp + bp->s.size == p->s.ptr){   //与上一块合并
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp){       //与下一块合并
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;       //如果bp已经与上一项合并，这句必须要有
    } else
        p->s.ptr = bp;
    freep = p;
}

//再写一遍free,特别是最后合并段
void t_free(void *ap)
{
    Header *p, *bp;

    bp = (Header *)(bp + 1);
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
        break;
    if (bp + bp->s.size == p->s.ptr){
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else 
        bp->s.ptr = p->s.ptr;
    if (p + p->s.size == bp){
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else
        p->s.ptr = bp;
    freep = p;
}


//调用malloc版
void *calloc1(unsigned n, unsigned size)
{
    int nelem, i = 1;
    Header *p;
    void *a[n];

    nelem = size / sizeof(Header) + 1;
    p = (Header *) malloc(n * size);
    for (a[0] = p; i < n; i++){
        a[i] = p + (nelem * sizeof(Header));
        a[i] = 0;
    }
    return (void *) *a;

}

//malloc修改版
void *calloc2(unsigned n, unsigned size)
{
    Header *prevp, *p;
    void *a[n];
    unsigned nunits, nelem;
    int i;

    nelem = size / sizeof(Header);
    nunits = (n * size) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL){       //首次使用，从空块开始
        base.s.ptr = prevp = freep = &base; //指向自己
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ;prevp = p, p = p->s.ptr){
        if (p->s.size >= nunits){           //足够大
            if (p->s.size == nunits)        //正好
                prevp->s.ptr = p->s.ptr;
            else {   //过大
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            for (a[0] = p; i < n; i++){
                a[i] = p + (nelem * sizeof(Header));
                a[i] = 0;
            }
            freep = prevp;          //更新空闲链表指针
            return (void *) *a; //留出头部空间，指向空闲区域
        }
        if (p == freep)  //到达一圈后，没找到足以容纳的空间
            if ((p = morecore(nunits)) == NULL)       //系统申请
                return NULL;
    }
}

/*分块的事情可以交给外面，calloc里将所有位置归零即可
    if ((p = q = malloc(nunits * sizeof(Header))) != NULL)
        for (i = 0; i < (nunit * (Header); i++)
            *p++ = 0;
    return q;  .....前面可以加一些对size的检错     */