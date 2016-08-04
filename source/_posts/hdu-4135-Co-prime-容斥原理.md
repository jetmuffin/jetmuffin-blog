title: hdu 4135 Co-prime 容斥原理
tags: [容斥原理,hdu]
categories: 算法
date: 2014-11-28 01:27:33
---

<span style="white-space:pre"></span>最近在看容斥原理的题目，这道题目是很典型的。

<span style="white-space:pre"></span>题目要求求[L,R]区间内与k互斥的数有多少个。

<span style="white-space:pre"></span>可以用容斥原理求[1,R]中与k互斥的个数，再减去[1,L-1]中与k互斥的个数。

<span style="white-space:pre"></span>容斥原求[1,n]中与k互斥的数的思路是：对于k，先分解质因数，放在prime[i]里，然后对于每个因数的各个组合，求[1,n]中能被其整除的个数，即n/prime[i]，然后不能直接相加。

<span style="white-space:pre"></span>比如说30的因数是2,3,5。求[1,20]中与30互斥的数，先要求20/2,20/3,20/5，然后求20/6,20/10,20/15，最后求20/30。

<span style="white-space:pre"></span>由2个因数组成的除数，要减，一个因数组成的，则加。

<span style="white-space:pre"></span>再讲怎么取到这每一个组合。用位运算表示最方便了。如i=3用011表示，就是要取prime[0]，prime[1]。

<span style="white-space:pre"></span>因此只要循环i := 1 -> 2^cnt (因数个数) ，j := 0 -> cnt -1 ，若 (1<<(j+1)) & 1 不为0，则要取到该数。如i = 3,要取到prime[0]和prime[1]，1<<1 = " 001"， 1<<2 = "010"，则符合。

<span style="white-space:pre"></span>最后说一下题目里的坑，因为a,b，n都是long long 范围，所以循环变量i需要也为long long ，且函数返回也为long long ，prime[]数组也是long long ，总之精度不能有错。

<span style="white-space:pre"></span>然后是long long 的问题，hdoj long long 用%lld居然会WA，然后卡了我好几次，最后改为%I64d过了。。

<!--more-->

<span style="white-space:pre"></span>代码：`#include <iostream>

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

const int maxn = 70;

LL prime[maxn];

LL a,b,n;

int cnt;

int t = 0;

LL solve(LL num){

	LL ans = 0;

	for(LL i = 1; i < (LL)1<<cnt; i++){

		LL flag = 0, tmp = 1;

		for(LL j = 0; j < cnt; j++){

			if(i & (1<<j)){

				tmp *= prime[j];

				flag ++; 

			}

		}

		if(flag & 1){

			ans += num/tmp;

		} else{		

			ans -= num/tmp;

		}

	}

	return num - ans;

}

int main()

{

	int T;

	cin>>T;

	while(T--){

		t++;

		scanf("%I64d%I64d%I64d",&a,&b,&n);

		cnt = 0;

		for(LL i = 2; i*i <= n;i++){

			if(n&&n%i == 0){

				prime[cnt++] = i;

					while(n&&n%i == 0){

					n /= i;

				}

			}

		}

		if(n > 1){

			prime[cnt++] = n;

		}

		printf("Case #%d: %I64d\n",t,solve(b)-solve(a-1));	

	}

	return 0;	

}`