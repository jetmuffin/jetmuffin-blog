title: 'Codeforces #285 C Misha and Forest 队列维护 解题报告'
tags: [CodeForces,队列,森林]
categories: 算法
date: 2015-01-14 23:04:10
---

题目大意为一个森林，对于每个节点记录它的度数degree和与它连接的点的异或和Sv。现在给出每个点的degree和Sv，求这个图有多少条边，并写出每条边的顶点。

分析：

首先从异或和出发，加入x^y^z = a，那么左右都异或x的话，(x^x)^y^z = a^x，即y^z = a^x，同理z = a^x^y。

其次由于是森林，肯定有叶子节点，找出这些叶子节点，他们的degree均为1，且Sv为父节点的编号，每遍历一个叶子节点，将其父节点度数-1，Sv异或叶子节点编号，直到找到最后一个节点即可。

最后，需要用队列维护，不然会T。

<!--more-->

代码：

`#include <iostream>  

#include <cstdio>  

#include <cstring>  

#include <algorithm>  

#include <queue>  

#include <vector>  

#include <map>  

using namespace std;  

struct node{  

    int degree,s,i;  

    int vis;  

    void output(){  

        cout<<i<<" "<<degree<<" "<<s<<endl;   

    }  

}v[70000];  

typedef pair<int, int> PII;  

vector<PII> edge;  

vector<PII>::iterator it;  

queue <int> q;  

int main()  

{  

    freopen("in.txt","r",stdin);  

    int n;  

    scanf("%d",&n);  

    int cnt = 0;  

    for(int i = 0 ; i < n;i++)  

    {  

        scanf("%d%d",&v[i].degree,&v[i].s);  

        if(v[i].degree == 1)  

            q.push(i);  

    }  

    while(!q.empty())  

    {  

        int i = q.front();  

        q.pop();  

        if(v[i].degree == 1)  

        {  

            int fa = v[i].s;  

            v[fa].s ^= i;  

            v[fa].degree --;  

            edge.push_back(make_pair(i,fa));  

            if(v[fa].degree == 1)  

                q.push(fa);               

        }     

    }  

    cout<<edge.size()<<endl;  

    for(it = edge.begin();it!=edge.end();it++)  

    {  

        PII pb = *it;  

        cout<<pb.first<<" "<<pb.second<<endl;  

    }   

}`