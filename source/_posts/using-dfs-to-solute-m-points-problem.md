title: 算m点问题，深搜解决
tags: [dfs]
categories: 算法题解
date: 2014-10-08 12:49:28
---

算法导论课后的一道题。

题目：给定k个正整数，用算数运算符+,-,*,/将这k个正整数链接起来，使最终的得数恰为m。

样例输入：

5 125

7 2 2 12 3

样例输出：

7 * 3 = 21; 21 * 12 = 252 ; 252 - 2 = 250; 250 / 2 = 125

简单的用深搜解决了，不考虑优化什么的了，数据多的话会超时，不过这不是A题就这样吧~XD

<!--more-->

代码：

```cpp
#include <iostream>  
#include <cstdio>  
#include <cstring>  
#include <string>  
#include <algorithm>  
#include <cmath>  
#include <vector>  
#include <map>  
#include <queue>  
#include <ctime>  

using namespace std;  
#define maxn 100  
int data[maxn],vis[maxn],num[maxn];  
char op[maxn];  
int n,m;  
int value = 0;  
int flag = 0;  
void output() {  
    int tmp = num[0];  
    for(int i = 1; i < n ; i++) {  
        cout<<tmp<<" ";  
        if(op[i] == '+') {
            cout<<"+"<<" ";  
            cout<<num[i]<<" = ";  
            tmp += num[i];  
            cout<<tmp<<";";  
        }  

        if(op[i] == '-') {
            cout<<"-"<<" ";  
            cout<<num[i]<<" = ";  
            tmp -= num[i];  
            cout<<tmp<<";";  
        }  

        if(op[i] == '*') {
            cout<<"*"<<" ";  
            cout<<num[i]<<" = ";  
            tmp *= num[i];  
            cout<<tmp<<";";  
        }  

        if(op[i] == '/') {  
            cout<<"/"<<" ";  
            cout<<num[i]<<" = ";  
            tmp /= num[i];  
            cout<<tmp<<";";  
        }  
    }  
}  

void dfs(int t) {  
    //cout<<t<<" "<<value<<" "<<endl;  
    if(t >= n) {
        if(value == m) {
            cout<<"found!"<<endl;  
            output();  
            flag = 1;  
        }  
    }  

    else {
        for(int i = 0; i < n ; i++) {
            if(t == 0) {
                if(!vis[i]) {
                    value = data[i];  
                    num[t] = data[i];  
                    vis[i] = 1;  
                    dfs(t + 1);  
                    vis[i] = 0;  
                }  
            }  

            else {
                if(!vis[i]) {
                    vis[i] = 1;
                    int tmp = value;
                    op[t] = '+';
                    num[t] = data[i];
                    value = tmp + data[i];
                    dfs(t + 1);
                    if(flag) return;

                    op[t] = '-';
                    num[t] = data[i];
                    value = tmp - data[i];
                    dfs(t + 1);
                    if(flag) return;

                    op[t] = '*';
                    num[t] = data[i];
                    value = tmp * data[i];
                    dfs(t + 1);
                    if(flag) return ;

                    if(data[i] != 0) {
                        op[t] = '/';
                        num[t] = data[i];
                        value = tmp / data[i];
                        dfs(t + 1);
                        if(flag) return;
                    }

                    value = tmp;
                    vis[i] = 0;
                } 
            }  
        }  
    }  
}  

int main() {  
    // freopen("in","r",stdin);  
    cin>>n>>m;
    
    for(int i = 0; i < n; i++)
        cin>>data[i];  
    
    memset(vis, 0, sizeof(vis));
    memset(num, 0, sizeof(num));
    memset(op, 0, sizeof(op));
    dfs(0);  

    if(!flag) cout<<"not found!"<<endl;
}
```