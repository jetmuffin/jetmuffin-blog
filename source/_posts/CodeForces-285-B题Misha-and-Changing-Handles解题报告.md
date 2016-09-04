title: 'CodeForces #285 B题Misha and Changing Handles解题报告'
tags: [CodeForces]
categories:  算法题解
date: 2015-01-14 22:48:12
---

本题的意思是，给定多个改名的查询，每个查询包括一个新名字和旧名字，一个人可以多次更改，最终得到一个新名字，求这些查询中一共有多少个人，并且输出他最初的名字和最后的名字。(1<=q<=100）

<!--more-->
```
input

5

Misha ILoveCodeforces

Vasya Petrov

Petrov VasyaPetrov123

ILoveCodeforces MikeMirzayanov

Petya Ivanov

output

3

Petya Ivanov

Misha MikeMirzayanov

Vasya VasyaPetrov123
```
考虑到q比较小，所以可以瞎搞。。直接对于每个节点设置sid和pid分别连接下一个名字和上一个名字，对于每一个查询，维护它的前驱和后继，最后便利一遍找到没有sid和没有pid的就是最初和最末的名字，最后根据循环找出每个初始名的结束名即可。

比的时候居然没注意到要用2*q存储，结果RE了，这点要注意。

代码：

```cpp
#include <iostream>  

#include <cstdio>  

#include <cstring>  

#include <algorithm>  

using namespace std;  

struct node{  

    string v;  

    int pid;  

    int sid;  

    void output(){  

        cout<<v<<" "<<pid<<" "<<sid<<endl;  

    }  

}f[1001];  

int main()  

{  

    freopen("in.txt","r",stdin);  

    int n;  

    while(cin>>n)  

    {  

        for(int i = 0; i < n ; i++)  

        {  

            f[i].pid = -1;  

            f[i].sid = -1;  

        }  

        int cnt = 0;  

        int tmp;  

        for(int i = 0 ;i < n; i++)  

        {  

            string a,b;  

            cin>>a>>b;  

            int j;  

            for(j = 0;j < cnt;j++)  

                if(a==f[j].v) break;  

            if(j==cnt)  

            {  

                f[cnt].v = a;  

                f[cnt].pid = -1;  

                f[cnt].sid = cnt + 1;  

                cnt++;  

                f[cnt].v = b;  

                f[cnt].pid = cnt-1;  

                f[cnt].sid = -1;  

                cnt++;  

            } else {  

                f[cnt].v = b;  

                f[cnt].pid = j;  

                f[j].sid = cnt;  

                f[cnt].sid = -1;  

                cnt++;  

            }  

        }  

//      for(int i = 0 ; i < cnt ;i++)  

//      {  

//          f[i].output();  

//      }  

        int ans = 0;  

        for(int i = 0 ; i < cnt;i++)  

        {  

            if(f[i].pid == -1)  

            {  

                ans++;  

            }  

        }  

        cout<<ans<<endl;  

        for(int i = 0 ; i < cnt;i++)  

        {  

            if(f[i].pid == -1)  

            {  

                int si = f[i].sid;  

                int pi ;  

                while(si!=-1)  

                {  

                    pi = si;  

                    si = f[si].sid;  

                }  

                cout<<f[i].v<<" "<<f[pi].v<<endl;  

            }  

        }         

    }  

}
```