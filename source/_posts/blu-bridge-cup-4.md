title: 蓝桥杯校内选拔第四题解题报告
tags: [蓝桥杯,dfs]
categories: 算法题解
date: 2015-02-03 16:21:28
---

题目：

今有7对数字：两个1，两个2，两个3，...两个7，把它们排成一行。

要求，两个1间有1个其它数字，两个2间有2个其它数字，以此类推，两个7之间有7个其它数字。如下就是一个符合要求的排列：

17126425374635

当然，如果把它倒过来，也是符合要求的。

请你找出另一种符合要求的排列法，并且这个排列法是以74开头的。

注意：只填写这个14位的整数，不能填写任何多余的内容，比如说明注释等。

思路：

题目是很简单的，去年蓝桥杯也有类似题目，当时不知道怎么写，现在会了= = 记录一下吧，思路就是纯搜索,dfs就OK了

<!--more-->

代码：

```cpp
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;
int f[15];
void dfs(int i,int v) {
	if(v == 0) {
		for(int k = 1 ; k <= 14; k++ )
			cout<<f[k];
		cout<<endl;
		return;
	}

	if(i > 14)
		return;

	for(int i = 1 ; i+v+1<=14 ;i++ ) {
		if(f[i] == 0 && f[i+v+1] == 0) {
			f[i] = v;
			f[i+v+1] = v;
			dfs(i,v-1);
			f[i] = 0;
			f[i+v+1] = 0;
		} 		
	}
}

int main() {
	memset(f,0,sizeof(f));
	dfs(1,7);
}
```