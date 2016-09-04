title: hdu 5392 Infoplane in Tina Town
tags: [数论,最小公倍数,群论,循环节]
categories: 算法题解
date: 2015-09-04 11:01:28
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=5392](http://acm.hdu.edu.cn/showproblem.php?pid=5392)

## 题意

给定一个置换a，求序列1-n经过多少次a置换能够再次变回序列1-n

## 分析

题意很明显，是求置换群的循环长度。

根据数论中的内容，循环长度等于所有循环节长度的最小公倍数，因此只要计算所有循环节的长度即可。

但是题目中提到需要对最后的答案进行求模处理，然而欧几里得算法求最小公倍数无法去模，因此要用质因数分解法进行求最小公倍数
<!-- more -->
复杂度：`O(nlog(n))`

## 代码

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
                }
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