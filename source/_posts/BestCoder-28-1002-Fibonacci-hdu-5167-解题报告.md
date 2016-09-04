title: 'BestCoder #28 1002 Fibonacci hdu 5167 解题报告'
tags: [BestCoder,hdu,dfs,fibonacci]
categories: 算法题解
date: 2015-02-03 15:20:35
---

题目很简单，就是给出一个n，判断n能否由斐波那契数相乘得到，n范围为1e9。

先判断了一下，该范围大概有40来个数，先预处理保存这些数即可。

一开始的思路是对于每个数直接进行dfs，判断能否能够达到该n，但是写的太烂，加上case T的范围是1e5，结果TLE了。之后看了看别人代码的思路，找到了一种比较好的方法。

<!--more-->

先预处理所有能达到的乘积，然后对于每次查询二分答案即可。测试了下，处理完所有范围的数，大概在80ms左右，看来还是不错的。

dfs的主体内容就是：

```
for i:= t to cnt 

val *= f[i];

dfs(i,val);
```

其中t为当前搜索的index，val为当前乘积值，再加上一些限制条件就可以了。

```cpp
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;
#define INF 1000000000
#define LL long long
LL f[100];
int vis[100];

int cnt;
int flag;
LL now = 1;
LL n;
vector<LL> v;
vector<LL>::iterator it;
void dfs(int t,LL val) {
	//cout<<t<<" "<<val<<endl; 
	v.push_back(val);
	if(t >= cnt)
		return;
	for(int i = t; i < cnt; i++) {
		LL tmp = val * f[i];
		if(tmp > INF)
			break;
		dfs(i,tmp);
	}
}

void init() {
	f[0] = 0;
	f[1] = 1;
	cnt = 2;
	for(int i = 2;f[i] < 100;i++) {
		f[i] = f[i-1] + f[i-2];
		if(f[i] > INF) break;
		cnt++;
	}	
	//cnt = 5;
    v.push_back(0);	
	dfs(3,1);
	sort(v.begin(),v.end());
	v.erase(unique(v.begin(),v.end()),v.end());
}

int main() {
	 #ifndef ONLINE_JUDGE
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    #endif
	init();
	int T;
	cin>>T;
	while(T--) {
		cin>>n;
		flag = 0;
		memset(vis,0,sizeof(vis));
		it = lower_bound(v.begin(),v.end(),n);
		if( it!= v.end() && *it== n) flag = 1;
		if(!flag)
			puts("No");
		else
			puts("Yes");
	}
}
```