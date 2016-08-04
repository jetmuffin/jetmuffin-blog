title: CodeForces 148D Bags of mice 概率dp
tags: [CodeForces,概率dp]
categories: 算法
date: 2014-10-14 17:47:35
---

开始看概率dp的内容，碰到算期望的题目，一般就要用概率dp了。这是道入门的概率dp，弄清楚每一步发生的事件和对应的概率，求出递推公式就OK了。

dp[i][j][0]表示轮到公主取，还剩下i只白鼠，j只黑鼠；

dp[i][j][1]表示轮到龙取，还剩下i只白鼠，j只黑鼠；

获胜的期望就是dp[w][b][0]。

dp[i][j][0] = p1  //公主抽到的是白鼠概率p1，结束

dp[i][j][0] += dp[i][j-1][1] * p2 //公主抽到的是黑鼠概率p2

dp[i][j][1] += dp[i-1][j-1][0] * p3 // 龙抽到的是黑鼠，吓跑一只白鼠概率p3

dp[i][j][1] += dp[i][j-2][0] * p4 //龙抽到黑鼠，吓跑一只黑鼠概率p4

再依次算概率 就OK了。

<!--more-->

代码：

`#include <iostream>

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

#define WE freopen("out.txt","w",stdout) 

#define N 1005

double dp[N][N][2]; 

int main()

{

	int w,b;

	cin>>w>>b;

	memset(dp,0,sizeof(dp));

	for(int i=0;i<=w;i++)

		for(int j=0;j<=b;j++)

		{

			if(i>0)

				dp[i][j][0] = i/(double)(i+j);

			if(j>0)

				dp[i][j][0] += j/(double)(i+j) * dp[i][j-1][1]  ;

			if(i>=1&&j>=1)

				dp[i][j][1] += j/(double)(i+j) *i/(double)(i+j-1) * dp[i-1][j-1][0]  ;

			if(j>=2)

				dp[i][j][1] += j/(double)(i+j) * (j-1)/(double)(i+j-1) * dp[i][j-2][0]  ;

		}

	  printf("%.9lf\n",dp[w][b][0]);

}`