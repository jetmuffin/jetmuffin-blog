title: hdu5011 西安邀请赛E题 Game 博弈论
tags: [hdu,ACM网络赛,博弈论]
categories: 算法
date: 2014-09-14 11:01:11
---

比赛前没有看过博弈论，也不知道Nim问题，这题写了一个多小时，做出来一个错误的结论。。硬伤。。

事实上只要把每个数求异或就可以出结果了。

<!--more-->

代码：
`#include<iostream>

using namespace std;

int main()

{

	int n;

	while(cin>>n)

	{

		long long a;

		int ans;

		cin>>ans;

		for(int i=1;i<n;i++)

		{

			cin>>a;

			ans ^= a;

		}

		if(ans) puts("Win");

		else puts("Lose");

	}

}`