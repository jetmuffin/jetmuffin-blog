title: hdu 5326 Work 2015多校联训第三场 并查集
tags: [并查集]
categories: 算法题解
date: 2015-08-01 09:49:19
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=5326](http://acm.hdu.edu.cn/showproblem.php?pid=5326)

## 题意

题意非常像裸并查集，每个员工都有一个直系上司，给定员工的管理状态，求管理k个下级员工的员工有多数个

## 分析

本题比并查集简单，不需要用到查询部分，只要进行合并，添加一个子节点时将节点合并至父节点，更新权值即可，因为数据量小，所以直接遍历一遍求`a[i] = k`的个数就行了

## 代码
```cpp
    #include <iostream>
    #include <cstring>
    #include <vector>
    #include <cstdio>
    using namespace std;

    #define MAXN 400
    int pre[MAXN];
    int a[MAXN];

    void merge(int i,int j){
        pre[j] = i;
        a[i]++;
        while(pre[i] != -1){
            a[pre[i]]++;
            i = pre[i];
        }
    }

    int main(){
        int n,k;
        while(scanf("%d%d",&n,&k) != EOF){
            memset(pre,-1,sizeof(pre));
            memset(a,0,sizeof(a));
            int x,y;
            for(int i=0; i<n-1; i++){
                scanf("%d%d",&x,&y);
                merge(x,y);
            }

            int cnt = 0;
            for(int i=1; i<=n; i++){
                if(a[i] == k)
                    cnt++;
            }
            cout<<cnt<<endl;

        }
        return 0;
    } 
```