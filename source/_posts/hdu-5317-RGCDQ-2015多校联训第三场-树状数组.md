title: hdu 5317 RGCDQ 2015多校联训第三场 树状数组
tags: [多校联训,因数分解,树状数组]
categories: 算法题解
date: 2015-08-01 10:01:00
---

## 代码
```cpp
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <math.h>
using namespace std;

const int MAXN = 1000010;
int c[MAXN];
int n;
bool test[MAXN];
int tot,factor_num;
int prim[500000];

int lowbit(int x)
{
    return x&(-x);
}

void add(int i,int val)
{
    while(i <= n)
    {
        c[i] = max(c[i],val);
        i += lowbit(i);
    }
}

int Max(int i)
{
    int s = 0;
    while(i > 0)
    {
        s = max(s,c[i]);
        i -= lowbit(i);
    }
    return s;
}

int a[MAXN];
int b[MAXN];
int ans[MAXN];

struct Node
{
    int l,r;
    int index;
}node[MAXN];

bool cmp(Node a,Node b)
{
    return a.l > b.l;
}

void getPrim()
{
    memset(test,true,sizeof(test));
    memset(prim,0,sizeof(prim));
    tot = 0;
    for (int i=2;i<=n;i++)
    {
        if (test[i])
        {
                    prim[tot] = i;
                    tot++;
        }
        for (int j=0;(j<tot && i*prim[j]<=n);j++)
        {
            test[i*prim[j]] = false;
            if (i%prim[j]==0)
            break;
        }
    }
}

void resolve(int now)
{
     factor_num = 0;
     for (int i=0;i<tot;i++)
     {
         if (test[now])
         {
               factor_num++;
               return;
         }
         if (now%prim[i]==0)
         {
               factor_num++;
         }
         while (now%prim[i]==0)
         {
               now=now/prim[i];
         }
         if (prim[i]>now)
            break;
      }
}

int main()
{
    int T;
    n = 0;
    scanf("%d",&T);
    for(int i=0;i<T;i++)
    {
        scanf("%d%d",&node[i].l,&node[i].r);
        node[i].index = i;
        if (node[i].r>n)
            n = node[i].r;
    }
    getPrim();
    for (int i=2;i<=n;i++)
    {
        resolve(i);
        a[i] = factor_num;
    }
    sort(node,node+T,cmp);
    int i = n;
    int j = 0;
    memset(b,0,sizeof(b));
    memset(c,0,sizeof(c));
    while(j < T)
    {
        while(i > 1 && i>= node[j].l)
        {
            for (int k=1;k*k<=a[i];k++)
            {
                if(a[i]%k==0)
                {
                    if(b[k]!=0)
                    {
                        add(b[k],k);
                    }
                    b[k] = i;
                    if(k!=a[i]/k)
                    {
                        if(b[a[i]/k]!=0)
                        {
                            add(b[a[i]/k],a[i]/k);
                        }
                        b[a[i]/k]=i;
                    }
                }
            }
            i--;
        }
        while(j<T && node[j].l>i)
        {
            ans[node[j].index]=Max(node[j].r);
            j++;
        }
    }
    for (int i=0;i<T;i++)
        printf("%d\n",ans[i]);
    return 0;
}
``` 