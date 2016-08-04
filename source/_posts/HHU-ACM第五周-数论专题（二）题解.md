title: HHU ACM第五周 数论专题（二）题解
tags: []
categories: '算法'
date: 2015-09-08 13:17:00
---

## 1000 Zball in Tina Town [HDOJ-5391](http://acm.hdu.edu.cn/showproblem.php?pid=5391)

### 题意

Z-ball在第n天会变为前一天的n倍，求最后的大小mod n，即求(n-1)!%n
<!-- more -->

### 思路

根据威尔逊定理(自行百度），

``` latex
    f(x)=
    \begin{cases}
    n-1, &x\in prime\cr 0, &x\not \in prime \end{cases}
```

此外有一个trick，即f(4)=2.
没看过威尔逊定理也没事，暴力求前几项打个表就能发现规律了

### 代码

``` cpp

#include <iostream>
#include <cstdio>
using namespace std;
bool is_prime(int x)  
{  
    for(int i=2;i*i<=x;i++)  
        if(x%i==0)return false;  
    return x!=1;  
}  
int main()  
{  
    int t;  
    scanf("%d",&t);  
    while(t--)  
    {  
        int n;  
        scanf("%d",&n);  
        if(n==4) printf("2\n");  
        else if(is_prime(n)) printf("%d\n",n-1);  
        else printf("0\n");  
    }  
    return 0;  
}  

```

## 1001 C Looooops [POJ-2115](http://poj.org/problem?id=2115)

### 题意

循环开始数为variable = A，循环结束条件为variable = B,每次variable + 2，最大数范围为2^k，求循环执行几次

### 思路

根据题意列出同余方程:

```latex
`C * n \equiv B (mod 2^{k}) 


其中`C`,`B`,`2^k`均已知，符合拓展欧几里得算法（自行百度）的条件因此我们直接可以套用拓展欧几里得算法解同余方程。

也有其他做法，不需要拓展欧几里得也能做出。

### 代码

```cpp
#include <iostream>
using namespace std;
#define LL long long
LL extgcd(LL a,LL b,LL &x,LL &y){
    if(b == 0){
        x=1;
        y=0;
        return a;
    }
    LL d = extgcd(b, a%b, y, x);
    y -= x*(a/b);
    return d;
}
int main(){
    LL A,B,C,K;
    while(~scanf("%lld%lld%lld%lld",&A,&B,&C,&K)){
        if(A==0 && B ==0 && C ==0 & K==0)
            break;
        LL x, y;
        LL m = (1LL << K);
        LL gcd = extgcd(C, m, x, y);
        LL b = (B - A + m) % m;
        LL t = m / gcd; 
        if(b % gcd == 0){
            LL ans = (b / gcd * x) % m; 
            ans = (m + ans) % m;
            ans = (ans % t + t) % t;
            cout<<ans<<endl;
        }else{
            cout<<"FOREVER"<<endl;
        }
    }
}
```
## 1002 Biorhythms [POJ-1006](http://poj.org/problem?id=1006)

### 题意

给出三个日期，分别对应于体力，情感，智力出现峰值的日期。然后再给出一个起始日期，要求从这一天开始，算出最少再过多少天后三个峰值同时出现。

### 思路

根据题意可以列出模线性方程

```latex
\begin{cases}
    x \equiv p (mod \28) \cr x \equiv e (mod \33) \cr x \equiv i (mod \35)\end{cases}
```
    
然后根据中国剩余定理（自行百度），可以直接套用模板做出，下面给出通用的中国剩余定理模板，实际解这题中很多值已经可以直接算出来了,**代码会更简单一些**

### 代码

```cpp
#include <iostream>
using namespace std;
#define LL long long
int m[3] = {23, 28, 33};
int a[3];
int extgcd(int a,int b,int &x,int &y){
    if(b == 0){
        x=1;
        y=0;
        return a;
    }
    int d = extgcd(b, a%b, y, x);
    y -= x*(a/b);
    return d;
}
int crt(int n){
    int M = 1;
    for(int i = 0; i < n; ++i)
        M *= m[i];
    int ret = 0;
    for(int i = 0; i < n; ++i){
        int x, y;
        int t = M/m[i];
        extgcd(t, m[i], x, y);
        ret = (ret + t *x * a[i]) % M;
    }
    return ret;
}
int main(){
    int p, e, i, d;
    int x,y;
    int cnt = 0;
    while(~scanf("%d%d%d%d",&p,&e,&i,&d)){
        cnt++;
        if(p==-1 && e ==-1 && i ==-1 & d==-1)
            break;
        a[0] = p;
        a[1] = e;
        a[2] = i;
        int ans = crt(3);
        ans = (ans - d) % 21252;
        if(ans <= 0)
            ans += 21252;
        printf("Case %d: the next triple peak occurs in %d days.\n",cnt,ans);
    }
}
```

## 1003 Infoplane in Tina Town [HDOJ-5392](http://acm.hdu.edu.cn/showproblem.php?pid=5392)

### 题意

给定置换A的序列，每次对初始序列[1,2,3,...,n]做A置换，求最少做几次A置换能够变回到原序列

### 分析

根据题意，要求的是A置换的循环长度，根据定理，循环长度即为置换中所有循环节长度的最小公倍数。因此只要求出各个循环节的长度，并求总的最小公倍数即可。然而因为需要取模，而常规欧几里得算法求gcd再求lcm求最小公倍数不可取模，因此需要用分解质因数方法去求最小公倍数。

将每个数分解成质因数p的幂次的乘积的形式，则每个质因数在lcm中出现的次数为在每个a[i]中出现次数的最大值，统计每个质因数p出现的次数，最后做一次乘积即可。

```latex
lcm = p_{1}^{b_{1}}\times p_{2}^{b_{2}}\times p_{3}^{b_{3}}\times ... p_{k}^{b_{k}}
```

### 代码
```cpp
#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
using namespace std;
#define LL long long
const int maxn = 3000010;
const LL mod = 3221225473;
int a[maxn];
int b[maxn];
int vis[maxn];
int n;

int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        memset(vis,0,sizeof(vis));
        memset(b,0,sizeof(b));
        for(int i = 1; i <= n; i++){
            scanf("%d",a+i);
        } 
        for(int i = 1 ; i <= n; i++){
            if(vis[i]) continue;
                int k = i;
                int len = 0;
                while(!vis[k]){
                    vis[k] = 1;
                    len++;
                    k = a[k];
                } //求出len为循环节长度
                for(int j = 2; j*j <= len; j++){
                    int cnt = 0;
                    while(len % j == 0){
                        cnt++;
                        len /= j;
                    }
                    b[j] = max(b[j], cnt);
                }
                if(len > 1)
                    b[len] = max(b[len], 1);                
                //求最小公倍数
        }
        LL ans = 1;
        for(int i = 2; i <= n; i++){
            for(int j = 1; j <= b[i]; j++)
                ans = ans * i % mod;
        }
        printf("%lld\n",ans);       
    }
}
```
   

## 1004 233 Matrix [HDOJ-5015](http://acm.hdu.edu.cn/showproblem.php?pid=5015)

### 题意

给定一个矩阵，矩阵的第一行为0，233,2333,23333.。。第一列为输入的序列，同时`ai,j = ai-1,j +ai,j-1( i,j ≠ 0)`，求a[n,m]。

### 分析

这题是一个很好的矩阵递推公式求解的题目，求解过程如下：

首先我们将矩阵的第一列进行一次转换：

```latex
\begin{pmatrix}
    0\\ 
    a[1]\\ 
    a[2]\\ 
    a[3]\\ 
    a[4]
    \end{pmatrix}

    \to
    \begin{pmatrix}
    23\\ 
    a[1]\\ 
    a[2]\\ 
    a[3]\\ 
    a[4]\\
    3
    \end{pmatrix}
```

    那么由第一列转换为第二列即可表示为：

```latex
    \begin{pmatrix}
    23*10+3\\ 
    a[1]+23*10+3\\ 
    a[2]+a[1]+23*10+3\\ 
    a[3]+a[2]+a[1]+23*10+3\\ 
    a[4]+a[3]+a[2]+a[1]+23*10+3
    \end{pmatrix}
```

    这样我们就能发现，`f[2,2] = f[1,2]+f[2,1]; f[3,2] = f[2,2]+f[3,1]` ....

    顺手一推即可得到转移矩阵A为：

```latex 
    \begin{bmatrix}
    10 &0 &0 &0 &0 &1\\
    10 &1 &0 &0 &0 &1\\
    10 &1 &1 &0 &0 &1\\
    10 &1 &1 &1 &0 &1\\
    10 &1 &1 &1 &1 &1\\
     0  &0 &0 &0 &0 &1\\
    \end{bmatrix}
```

然后呢可以得到`f[2]（第二列）= A*f[1]`,`f[n]=A*f[n-1]`, `f[n] = A^m*f[0]`

然后套用矩阵快速幂就可以了

### 代码

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
const int mod = 10000007;
struct matrix
{
  long long m[15][15];
  matrix()  
  {  
      memset(m,0,sizeof(m));  
  } 
};
int n,m;

matrix mul(matrix a, matrix b){
  matrix c;
  for(int i = 1 ; i <= n+2; i++)
    for(int j = 1; j <= n+2; j++)
    {
      for(int k = 1; k <= n+2; k++)
      {
        c.m[i][j] = (c.m[i][j] + a.m[i][k]*b.m[k][j] ) %mod;
      }
    }
  return c;
} 
matrix pow(matrix a, int k){
  matrix b;
  for(int i = 1; i <= n+2; i++)
    b.m[i][i] = 1;
  while(k >= 1){
    // cout<<k<<endl;
    if(k&1)
      b = mul(b,a);
    a = mul(a,a);
    k = k/2;
  }
  return b;
}

int main(){
  while(~scanf("%d%d",&n,&m)){
    matrix ans,base;
    ans.m[1][1]=23;  
    for(int i=1;i<=n;i++)  
    {  
        scanf("%d",&ans.m[i+1][1]);  
    }  
    ans.m[n+2][1]=3;  
    for(int i=1;i<=n+1;i++)base.m[i][1]=10;  
    for(int i=1;i<=n+2;i++)base.m[i][n+2]=1;  
    for(int i=2;i<=n+1;i++)  
    {  
        for(int j=2;j<=i;j++)base.m[i][j]=1;  
    }  
    base = pow(base,m);
    ans = mul(base,ans);
    cout<<ans.m[n+1][1]<<endl; 
  }
}
```