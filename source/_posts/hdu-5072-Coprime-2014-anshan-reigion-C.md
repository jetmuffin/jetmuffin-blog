Ctitle: hdu5072 Coprime 2014鞍山现场赛C题 容斥原理+单色三角
tags: [容斥原理,单色三角]
date: 2014-11-28 19:00:04
categories: 算法题解
---

鞍山赛区比赛的时候这题没有任何思路，比赛结束后clj提到了单色三角，后来在做白书的训练指南时候看到了这点，然后就把这题写一写。

用到的几个知识点：

1. 单色三角

	平面中n个点没有三点共线，每两个点染白边或者染黑边，求单色三角形的数量。

	事实上只要求非单色三角形，因为一个公共点的两条异色边总是唯一且对应一个非单色三角形，因此第i个点连接了a条白边，n-1-a条黑边，这些边属于a*(n-1-a)个非单色三角形，因此答案是C(3,n)-sigma(ai*(n-1-ai))。

2. 容斥原理

	这里用的容斥原理和hdu4135的题目相似。即求1-n中有多少个数与k互质。

	求法就是将k分解质因数，然后对于其质因数的每一个组合，求多少个数可以被整除，最后用容斥原理相加可得不互质的数量，也就能得到答案了。

	<!--more-->

	如n=20,k=30。质因数为2,3,5，所以依次求20/2,20/3,20/5,20/6,20/10,20/15,20/35，用到偶数个因数为减，奇数个加。

3. 打素数表

	这个就不用提了，作用是用来分解质因数。

4. 提前处理

	这一点比较重要，因为是要算这n个不同的数中与x互质的数。所以事先处理这些数中与（1-100000）中数不互质的数的个数。num[i]表示数列中，能被i整除的数的个数。

代码：

```cpp
#include <stdio.h>

#include <iostream>

#include <map>

#include <set>

#include <bitset>

#include <list>

#include <stack>

#include <vector>

#include <math.h>

#include <string.h>

#include <queue>

#include <string>

#include <stdlib.h>

#include <algorithm>

// #define LL __int64

#define LL long long

#define eps 1e-9

#define PI acos(-1.0)

using namespace std;

const LL INF = 1<<30;

const int maxn = 100010;

int n;

int prime[maxn];

int factor[maxn][30];

int num[maxn];

int a[maxn];

int cnt[maxn];

void getPrime(){

	memset(prime,0,sizeof(prime));

	for(int i=2; i<=maxn; i++)

	{

		if(!prime[i]) prime[++prime[0]]=i;

		for(int j=1; j<=prime[0]&&prime[j]<=maxn/i; j++)

		{

			prime[prime[j] * i] =1;

			if(i%prime[j] == 0) break;

		}

	}

}

void getFactor(int val, int pos){

	int tmp = val;

	for(int i = 1; i <= prime[0] &&prime[i]* prime[i] <= tmp; i++)

	{

		if(tmp % prime[i] == 0)

		{

			factor[pos][cnt[pos]++] = prime[i];

			while(tmp % prime[i] == 0)

				tmp /= prime[i];

		}

		if(tmp == 1) break;

	}

	if(tmp > 1)

		factor[pos][cnt[pos]++] = tmp;

	// cout<<val<<": ";

	// for(int i = 0;i<cnt[pos];i++){

	// 	cout<<factor[pos][i]<<" ";

	// }

	// cout<<endl;

}

void init(){

	for(int i =2; i<100000; i++)

		for(int j= i+i;j < 100000; j+=i)

		{

			num[i] += num[j]; 

		}

}

int main(){

//	freopen("data2","r",stdin);

	getPrime();

	int T;

	cin>>T;

	while(T--){

		memset(cnt,0,sizeof(cnt));	

		memset(num,0,sizeof(num));

		int n;

		scanf("%d",&n);

		for(int i = 1; i <= n; i++)

		{

			scanf("%d",&a[i]);

			num[a[i]]++;

		}

		init();

		int ans = 0;

		for(int i = 1; i <= n; i++)

		{

			getFactor(a[i],i);

			int ansNow = 0;

			for(int j = 1; j < 1<<cnt[i]; j++)

			{

				int flag = 0, tmp = 1;

				for(int k = 0; k < cnt[i]; k++)

				{

					if(j&(1<<k))

					{

						flag ++;

						tmp *= factor[i][k];

					}

				}

				if(flag &1)

					ansNow += num[tmp] -1;

				else

					ansNow -= num[tmp] -1;

			}

			ans += ansNow*(n-1-ansNow);

		}

		ans = n*(n-1)*(n-2)/6-ans/2;

		cout<<ans<<endl;

	}

}
```