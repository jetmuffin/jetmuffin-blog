title: stl set和map应用记录
tags: [stl,map]
categories: 算法分析
date: 2014-10-22 03:52:45
---

<span style="white-space:pre"></span>无聊水题，发现map容器使用的一块儿知识欠缺，就是很水的用map进行计数，赶紧补上。

<!--more-->


```
#include<iostream>
#include<map>
using namespace std;
int main()
{
    map<int,int> s;
    int n;
    while(scanf("%d",&n)&&n)
    {
        s.clear();
        while(n--)
        {
            int x;
            scanf("%d",&x);
            s[x]++;
        }
        map<int,int>::iterator i;
        for(i=s.begin();i!=s.end();i++)
        {
        	cout<<i->first<<" "<<i->second<<endl;
        }
    }
    return 0;
}
```