title: hdu 5327 Olympiad 2015多校联训第四场 水题
tags: [水题,多校联训]
categories: 算法
date: 2015-08-01 10:08:39
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=5327](http://acm.hdu.edu.cn/showproblem.php?pid=5327)

## 题意

求`[a,b]`范围内，各位都不重复的数字的个数

## 分析

求各位不重复，逐位分解打标记即可；然后对于个数，可以直接打表`f[i]`表示`1-i`中符合条件的数的个数，然后对于每次查询，输出`f[b]-f[a-1]`即可

## 代码

    #include <iostream>
    #include <cstring>
    #include <cstdio>
    using namespace std;
    const int maxn = 100005;
    int f[maxn];
    int a[10];
    int check(int n){
      memset(a,0,sizeof(a));
      while(n){
        int t = n%10;
        n /= 10;
        a[t] ++;
        if(a[t] >= 2){
          return 0;
        }
      }
      return 1;
    }
    int main(){
      f[0] = 0;
      for(int i = 1; i <= maxn; i++)
        if(check(i))
          f[i] = f[i-1] + 1;
        else
          f[i] = f[i-1];
      int t;
      cin>>t;
      while(t--){
        int a,b;
        cin>>a>>b;
        cout<<f[b]-f[a-1]<<endl;
      }
    }
    