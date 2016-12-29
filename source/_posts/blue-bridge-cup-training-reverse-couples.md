title: 蓝桥杯算法训练-逆序对 解题报告
tags: [蓝桥杯,归并排序,动态规划,逆序对]
categories: 算法题解
date: 2015-03-28 11:09:21
---

### 问题描述

大体意思是，根据输入构建一棵二叉树，其中以其叶子节点从左到右为一数列，为了使得该数列的逆序对最少，可以对每个非叶子节点进行调换左右字数操作，求最少的逆序对。

### 算法思路

题目标明用平衡二叉树进行处理，可以选择用Splay树或者AVL树进行处理，考虑到之前求逆序对的题目原型一般用归并排序进行，所以这里我打算用**归并排序**加**动态规划**处理。

<!--more-->

由于题目中特殊的树结构，所以将传统的归并排序的二分改为递归到叶子节点，再合并到根节点。根据其**最优子结构**，要使得整体逆序对最少，也就是每个子树的逆序对最少，以此类推。可以很容易得到递推公式：
> f(root) = f(left) +　ｆ(right) + min(merge(left , right) , merge(right , left));

合并时的思路为，a[i]和b[j]分别表示两个序列的相应元素，若a[i] > b[j]，则a[i]之后的数均大于b[j]，因为该序列为从小到大排序的，这样就可以统计逆序对了。

同理可以统计顺序对，顺序对则对应了调换子树后的逆序对，比较顺序对和逆序对的大小，然后求f(root)，同时合并序列至根节点。

### 代码

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
#define MP make_pair
#define mod 1000000007
#define eps 1e-8
#define pi acos(-1.0)
#define inf 0x3fffffff
#define maxn 400005

struct node{
	vector<int> v;
	LL min;
}; 

LL ans;

LL maxx=0;

LL maxxx = 0;

int flag = 0;

node merge(node l,node r);

node build_tree(LL i){
//	printf("node %d:\n",i);
	int x;
	scanf("%d",&x);	
	node t;
	t.min = 0;

	if(x != 0)
		t.v.push_back(x);
	else{
		node l = build_tree(i+1);
		node r = build_tree(i+1);
		t = merge(l,r);
	}

	if(i == 1)
		ans = t.min;	
	return t;
}

node merge(node l,node r){
	if(l.v.size() > maxx)
		maxx = l.v.size();

	if(r.v.size() > maxx)
		maxx = r.v.size();

	int i = 0 ,j = 0;
	LL l_cnt = 0,r_cnt = 0;
	vector<int>tmp;

	while(i < l.v.size() && j < r.v.size()){
		if(l.v[i] == r.v[j]){
			tmp.push_back(l.v[i]);
			tmp.push_back(r.v[j]);
		} else if(l.v[i] < r.v[j]){
			r_cnt += r.v.size() - j;
			tmp.push_back(l.v[i++]); 
		} else{
			l_cnt += l.v.size() - i;
			tmp.push_back(r.v[j++]);
		}
	}

	while(i < l.v.size())
		tmp.push_back(l.v[i++]);
	while(j < r.v.size())
		tmp.push_back(r.v[j++]);
	node root;
	root.min = l.min + r.min + min(l_cnt,r_cnt);
	for(int k = 0; k < tmp.size();k++){
//		printf("%d ",tmp[k]);
		root.v.push_back(tmp[k]);
	}
//	printf("\n");
	return root;
}

int main() {
	#ifndef ONLINE_JUDGE
	    freopen("in.txt","r",stdin);//freopen("output.txt","w",stdout);
	#endif
	LL n;
	scanf("%d",&n);
	build_tree(1);
	cout<<ans<<endl;
}
```

### 结果

最后提交OJ的时候发现25组只通过了12组，48分，也就是说该方法还是有问题，理论上复杂度为`o(nlogn)`，但是数据过强，所以待我习得Splay大法后写一份Splay的解法。