title: '[转] 广义Fibonacci数列找循环节'
tags: [数论,循环节]
categories: 算法分析
date: 2015-09-20 09:23:00
---

## 出处

转自[ACDreamer的博客](http://blog.csdn.net/ACdreamers)

## 问题

给定![img-1](http://img.blog.csdn.net/20140512141202250)，满足![img-2](http://img.blog.csdn.net/20140512141344500)，求![img-3](http://img.blog.csdn.net/20140512141547343)的循环节长度。
<!-- more --> 

## 分析

我们知道矩阵的递推关系如下:

![img-4](http://img.blog.csdn.net/20140512141934171)

然后继续有:

![img-5](http://img.blog.csdn.net/20140512142241625)

那么，现在的问题就转化为求最小的![img-6](http://img.blog.csdn.net/20140512142408500)，使得![img-7](http://img.blog.csdn.net/20140512142715562)

所以我们可以先找出符合条件的一个![img-8](http://img.blog.csdn.net/20140512142408500)，然后枚举它的因子，找最小的。设

![img-9](http://img.blog.csdn.net/20140512143016218)

为了好解决问题，我们需要对矩阵![img-10](http://img.blog.csdn.net/20140512143448171)进行相似对角化，即![img-11](http://img.blog.csdn.net/20140512143448171)的特征值

解得![img-11](http://img.blog.csdn.net/20140512143448171)的特征值为![img-12](http://img.blog.csdn.net/20140512144649265)

也就是说![img-12](http://img.blog.csdn.net/20140512143448171)的相似对角矩阵![img-13](http://img.blog.csdn.net/20140512144531812)为

![img-14](http://img.blog.csdn.net/20140512144745468)

因为我们知道![img-15](http://img.blog.csdn.net/20140512145327265)，所以当![img-16](http://img.blog.csdn.net/20140512145502218)时，![img-17](http://img.blog.csdn.net/20140512145712093)，由于

![img-18](http://img.blog.csdn.net/20140512145910453)

继续得到

![img-19](http://img.blog.csdn.net/20140512150102359)

设![img-20](http://img.blog.csdn.net/20140512150213093)，那么分情况讨论：

1.  ![img-21](http://img.blog.csdn.net/20140512151050140)是模![img-22](http://img.blog.csdn.net/20140512151151359)的二次剩余，由费马小定理得![img-23](http://img.blog.csdn.net/20140512151341546)时，![img-24](http://img.blog.csdn.net/20140512151440765)
2.  ![img-24](http://img.blog.csdn.net/20140512151050140)是模![img-22](http://img.blog.csdn.net/20140512151151359)的二次非剩余，则有

    ![img-25](http://img.blog.csdn.net/20140512153301078),根据欧拉准则有

    ![img-26](http://img.blog.csdn.net/20140512153500734)

    那么继续得到

    ![img-27](http://img.blog.csdn.net/20140512153750203)

    然后由费马小定理有![img-28](http://img.blog.csdn.net/20140512153930718)，同理有![img-29](http://img.blog.csdn.net/20140512154020390)所以，当![img-30](http://img.blog.csdn.net/20140512154134531)时，![img-31](http://img.blog.csdn.net/20140512154236328)
3.  ![img-32](http://img.blog.csdn.net/20140512154416203)时，由于![img-33](http://img.blog.csdn.net/20140512154508375)不存在，所以无法完成相似对角化，好在![img-34](http://img.blog.csdn.net/20140512154416203)情况不存在。

所以综上所述：

*   ![img-35](http://img.blog.csdn.net/20140512151050140)是模![img-36](http://img.blog.csdn.net/20140512151151359)的二次剩余时，枚举![img-37](http://img.blog.csdn.net/20140512151341546)的因子
*   ![img-35](http://img.blog.csdn.net/20140512151050140)是模![img-36](http://img.blog.csdn.net/20140512151151359)的二次非剩余时，枚举![img-38](http://img.blog.csdn.net/20140512154134531)的因子

找最小的因子![img-39](http://img.blog.csdn.net/20140512155118687)使得

![img-40](http://img.blog.csdn.net/20140512155210937)

成立

## 代码
```cpp
#include <iostream>
#include <string.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>

using namespace std;
typedef long long LL;
const int N = 2;
const LL MOD = 1000000007;

LL fac[2][505];
int cnt,ct;

LL pri[6] = {2, 3, 7, 109, 167, 500000003};
LL num[6] = {4, 2, 1, 2, 1, 1};

struct Matrix
{
    LL m[N][N];
} ;

Matrix A;
Matrix I = {1, 0, 0, 1};

Matrix multi(Matrix a,Matrix b)
{
    Matrix c;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            c.m[i][j]  =0;
            for(int k=0; k<N; k++)
            {
                c.m[i][j] += a.m[i][k] * b.m[k][j];
                c.m[i][j] %= MOD;
            }
        }
    }
    return c;
}

Matrix power(Matrix A,LL n)
{
    Matrix ans = I, p = A;
    while(n)
    {
        if(n & 1)
        {
            ans = multi(ans,p);
            n--;
        }
        n >>= 1;
        p = multi(p,p);
    }
    return ans;
}

LL quick_mod(LL a,LL b)
{
    LL ans = 1;
    a %= MOD;
    while(b)
    {
        if(b & 1)
        {
            ans = ans * a % MOD;
            b--;
        }
        b >>= 1;
        a = a * a % MOD;
    }
    return ans;
}

LL Legendre(LL a,LL p)
{
    LL t = quick_mod(a,(p-1)>>1);
    if(t == 1) return 1;
    return -1;
}

void dfs(int dept,LL product = 1)
{
    if(dept == cnt)
    {
        fac[1][ct++] = product;
        return;
    }
    for(int i=0; i<=num[dept]; i++)
    {
        dfs(dept+1,product);
        product *= pri[dept];
    }
}

bool OK(Matrix A,LL n)
{
    Matrix ans = power(A,n);
    return ans.m[0][0] == 1 && ans.m[0][1] == 0 &&
           ans.m[1][0] == 0 && ans.m[1][1] == 1;
}

int main()
{
    fac[0][0] = 1;
    fac[0][1] = 2;
    fac[0][2] = 500000003;
    fac[0][3] = 1000000006;
    LL a,b,c,d;
    while(cin>>a>>b>>c>>d)
    {
        LL t = a * a + 4 * b;
        A.m[0][0] = a;
        A.m[0][1] = b;
        A.m[1][0] = 1;
        A.m[1][1] = 0;
        if(Legendre(t,MOD) == 1)
        {
            for(int i=0; i<4; i++)
            {
                if(OK(A,fac[0][i]))
                {
                    cout<<fac[0][i]<<endl;
                    break;
                }
            }
        }
        else
        {
            ct = 0;
            cnt = 6;
            dfs(0,1);
            sort(fac[1],fac[1]+ct);
            for(int i=0;i<ct;i++)
            {
                if(OK(A,fac[1][i]))
                {
                    cout<<fac[1][i]<<endl;
                    break;
                }
            }
        }
    }
    return 0;
}
```

    