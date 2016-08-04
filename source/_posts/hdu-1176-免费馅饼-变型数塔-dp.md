title: hdu 1176 免费馅饼 变型数塔 dp
tags: [hdu,dp,数塔]
categories: 算法
date: 2015-06-27 17:56:00
---

## 分析

题目：[题目链接](http://acm.hdu.edu.cn/showproblem.php?pid=1176)

按题意，t时间会落下一个馅饼在x位置，而处于x'位置的gameboy只能拿到x'+1、x'、x'-1三个位置的馅饼，若将馅饼下落信息用一个数组f[t][x]，发现对于每个i，j，gameboy只能取得f[i+1][j+1]，f[i+1][j-1]，f[i+1][j]三个值，要使得最后取得的值最大，一看这是典型的数塔问题，然后就可以用dp轻松解决了

注意：由于数组每层在计算后不再使用，所以直接用原数组进行滚动，从下往上将结果累加即可。

## 代码

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

#define MP make_pair

#define mod 1000000007

#define eps 1e-8

#define pi acos(-1.0)

#define inf 0x3fffffff

#define maxn 200055 

int f[maxn][20]; 

int maxx(int a,int b,int c){

	return max(max(a,b),c);

}

int main()

{

	#ifndef ONLINE_JUDGE

	    freopen("in.txt","r",stdin);

		//freopen("out.txt","w",stdout);

	#endif

	int n;

	while(~scanf("%d",&n)&&n){

		int t,x;

		memset(f,0,sizeof(f));

		int mx = 0;

		for(int i = 0 ; i < n; i++){

			scanf("%d %d",&x,&t);

			f[t][x+1] ++;

			if(t > mx) mx = t; 

		}

		for(int i = mx -1 ; i > 0; i--){

			for(int j = 1; j <= 11;j++){

				f[i][j] += maxx(f[i+1][j],f[i+1][j-1],f[i+1][j+1]);

			}

		}

		printf("%d\n",maxx(f[1][5],f[1][6],f[1][7]));

	}

}`