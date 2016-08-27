title: 2014 Facebook Hacker Cup 资格赛 第二题 二进制模拟
tags: [FaceBook Hacker Cup,二进制]
categories: 算法题解
date: 2015-01-14 18:27:35
---

Facebook Hacker Cup资格赛的第二题，题目的意思问，给定的三个指标Ga,Gb,Gc，给定最多20种食物，能否任意组合刚刚好达到该指标。

范围不是很大，20的话怎么搞都行，于是敲了一个二进制模拟去枚举。。过了

思路类似容斥原理里的枚举因子，例如 i = 3 = '011' 即选中第一个和第二个。

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
#define LL long long  
#define ULL unsigned long long  
//#define mod 1000000007  
#define eps 1e-8  
#define MP make_pair  
#define REP(i,a,b) for(int i = a; i < b; ++i)  
#define RREP(i,a,b) for(int i = b; i > a ; --i)  
#define RE freopen("in.txt","r",stdin)  
//#define WE freopen("out.txt","w",stdout)   

struct node {  
    int a,b,c;  
    void output(){  
        cout<<a<<" "<<b<<" "<<c<<endl;  
    }  
};   

node g;  
node f[21];  
int cnt = 0;  

int solve(int count){    
    for(int i = 1; i < (int)1<<count; i++){    
        int a,b,c;  
        a=b=c=0;    
        for(int j = 0; j < count; j++){    
            if(i & (1<<j)){    
                a += f[j].a;  
                b += f[j].b;  
                c += f[j].c;  
            }    
            if(a==g.a&&b==g.b&&c==g.c) return 1;  
            else if(a>g.a||b>g.b||c>g.c) break;  
        }    
    }    
    return 0;  
}    

int main() {  
    freopen("in.txt","r",stdin);  
    int T;  
    cin>>T;  
    while(T--) {  
        cnt++;  
        int n;  
        scanf("%d%d%d",&g.a,&g.b,&g.c);  
        scanf("%d",&n);  
        for(int i = 0 ; i < n;i++) {
            scanf("%d%d%d",&f[i].a,&f[i].b,&f[i].c);  
        }  
        int flag = 0;  
        flag = solve(n);  
        if(flag)  
            cout<<"Case #"<<cnt<<": yes"<<endl;  
        else  
            cout<<"Case #"<<cnt<<": no"<<endl;  
    }  
}
```