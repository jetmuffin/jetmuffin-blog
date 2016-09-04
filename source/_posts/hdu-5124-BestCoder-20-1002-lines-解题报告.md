title: 'hdu 5124 BestCoder #20 1002 lines 解题报告'
tags: [标记法]
categories: 算法题解
date: 2014-11-30 06:11:56
---

<span style="white-space:pre"></span>这道题和以前做过的一道很像，然后哗啦哗啦就敲了提交了，然后pretest pass了 ，交了以后发现有问题，我的方法只是对一维数组进行标记然后扫描一遍，但是对于1e9的数据不仅爆数组又会T。想了一会儿怎么改进，最后还是卡这了。

<span style="white-space:pre"></span>赛后想了想，做一个简单的映射就行了，在原来的数组上相当于再加一层数组，对当前数组的index也就是上层数组的value进行标记扫描，就好过了。。。

<span style="white-space:pre"></span>将线段的x和y都放在一个数组v里，然后对2*n大小的v数组排序，然后对于每一个x，二分查找v数组的lower_bound，然后标记++，对于每一个y,二分查找v数组的lower_bound，然后标记--，最后扫描一遍OK了。。

<!--more-->

<span style="white-space:pre"></span>代码：


```cpp
#include <iostream>

#include <cstdio>

#include <cstring>

#include <algorithm>

using namespace std;

const int maxn = 100005;

int a[maxn],b[maxn];

int f[maxn * 2];

int v[maxn * 2];

int main()

{

   freopen("in.txt","r",stdin);

    int T;

    cin>>T;

    while(T--)

    {

        int n;

        memset(a,0,sizeof(a));

        memset(b,0,sizeof(b));

        memset(f,0,sizeof(f));

        memset(v,0,sizeof(v));

        scanf("%d",&n);

        for(int i = 0 ; i <n ; i++)

        {

            scanf("%d%d",&a[i],&b[i]);

            f[i] = a[i];

            f[i+n] = b[i];

        }

        sort(f,f+n*2);

        for(int i = 0; i < n ; i++)

        {

                int aa = lower_bound(f, f+2*n,a[i])-f;

                int bb = lower_bound(f, f+2*n, b[i])-f;

                v[aa]++;

                v[bb+1]--;

        }

        int tmp = 0,ans= 0;

        for(int i = 0 ; i < n*2;i++)

        {

            tmp += v[i];

            ans = max(ans,tmp);

        }

        printf("%d\n",ans);

    }

    return 0;

}
```