title: hdu 4549 费马小定理 矩阵快速幂
tags: [数论,费马小定理,矩阵快速幂]
categories: 算法题解
date: 2015-02-04 15:21:49
---

题目是斐波那契的变式，F[n]=F[n-1]*F[n-2]，F[0]=a，F[1]=b。

可以推出最后的ans = a^x * b^y，然后x和y都是斐波那契相邻的两项，x=f(n-1)，y=f(n)。

斐波那契和求幂都可以用快速幂搞定，中间数字特别大，快速幂过程容易溢出，然后WA+TLE了好几发，看了看discuss，群巨建议用费马小定理，查阅了一下，当a和p互质且p是质数时，a^(p-1)≡1，然后变式就是a^p=a^(p%Euler(MOD))%MOD，至于为什么，我之后还会再研究研究。。最近看数论比较有效率。

<!--more-->

这样的话，就很容易可以推出

c = a^(f(n-1)%1000000006)%1000000007;

d = b^(f(n)%1000000006)%1000000007;

ans = c*d;

代码：

```cpp
#include <iostream>

#include <cstring>

using namespace std;

#define LL long long

const int N = 2;

const long long MOD = 1000000007;

struct Mat{

	LL m[N][N];

};

Mat operator * (Mat a,Mat b){

	Mat c;

	memset(c.m,0,sizeof(c.m));

	for(int i = 0 ; i < N; i++)

		for(int j = 0 ; j < N; j++)

			for(int k = 0 ; k < N; k++)

			{

				c.m[i][j] = (c.m[i][j] + a.m[i][k] * b.m[k][j]) % (MOD-1);

			}

	return c;

}

Mat operator ^ (Mat a,LL k){

	Mat c;

	memset(c.m,0,sizeof(c.m));

	for(int i = 0; i < N; i++)

			c.m[i][i] = 1;

	while(k){

		if(k&1)

			c = c * a;

		a = a * a;

		k >>= 1;

	}

	return c;

}

LL quick_pow(LL a,LL n){

	LL tmp = 1;

	while(n){

		if(n&1)

			tmp = tmp * a % MOD;

		n >>= 1;

		a = a * a % MOD;

	}

	return tmp;

}

int main()

{

	freopen("in.txt","r",stdin);

	LL a,b,n;

	while(~scanf("%I64d%I64d%I64d",&a,&b,&n))

	{

		if(n==0) printf("%I64d\n",a);

	    else if(n==1)printf("%I64d\n",b);

	    else{

			Mat base = {1,1,1,0};

			base = base ^ (n-1);

			LL c = base.m[0][0];

			LL d = base.m[0][1];

			LL ans = (quick_pow(a,d) * quick_pow(b,c))%MOD;

	//		cout<<c<<" "<<d<<endl;

		printf("%I64d\n",ans);		    	

	    }

	}

}
```