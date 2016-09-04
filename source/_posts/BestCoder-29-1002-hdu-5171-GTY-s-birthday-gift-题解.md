title: "BestCoder #29 1002 hdu 5171 GTY's birthday gift 题解"
tags: [矩阵快速幂,斐波那契]
date: 2015-02-07 21:56:20
categories: 算法题解
---

假期放松了太多，状态太差，水题都WA了好几发。。

先是第一题，思路很简单，取个对数判断大小就行了，然后脑抽了居然直接用“=="进行判断，事实上应该要用精度判断，然后卡到第二题做出来才想到。。ORZ

然后是第二题，读题后很容易可以发现最后的答案是取最大的2个数a,b，进行相加，然后得到第三个数a+b，再相加得到a+2b，反复下去类似斐波那契数列。然后可以得出a的系数是从斐波那契的某一项开始的前n项和-1，b同理是前某项和-2，然后用矩阵快速幂o(log(n))轻松搞定斐波那契，再加上数列所有数的和即可。

官方题解是直接从线性方程推出转移矩阵，然后根据三阶矩阵快速幂求得解，同样可用。

<!--more-->

这里值得一提的是，我竟然直接无视题目中的**mod 10000007**，然后想当然地感觉这个数会按往常一样取int范围内最大的质数**1000000007**，然后按着事先打好的模板，结果failed final test了。。这也警告之后不能按照打好的模板来。。还是得自己纯手打好。。

HACK时看到好多人直接for循环跑。。交了两发，都Invalid Input的了，听群里人才知道。。数据最后要换行。。ORZ

本来这场2题保证涨分，紫名近在眼前。。可惜啊，还是要努力啊！

代码：

```cpp
#include <iostream>

#include <cstring>

#include <algorithm>

using namespace std;

#define LL long long

const int N = 2;

const long long MOD = 10000007;

LL a[100005];

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

                c.m[i][j] = (c.m[i][j]%MOD + (a.m[i][k]%MOD) * (b.m[k][j]% MOD) % MOD) % MOD;

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

	LL n,k;

	while(~scanf("%I64d%I64d",&n,&k))

	{

		LL mx = 0;

		LL mxx = 0;

		LL sum = 0;

		for(int i = 0 ; i <n;i++)

		{

			scanf("%I64d",&a[i]);

			sum = (sum + a[i])%MOD;

			if(a[i]>mx)

			{

				mxx = mx;

				mx = a[i];

			}

			else if(a[i]>mxx)

				mxx = a[i];

		}

		sort(a,a+n);

		mx = a[n-1]% MOD;

		mxx = a[n-2]% MOD;

		//cout<<mx<<" "<<mxx<<endl;

		//cout<<sum<<endl;

		Mat base = {1,1,1,0};

        Mat mm = base ^ (k+2);

        LL c = mm.m[0][1]-1 % MOD;

        LL d = mm.m[0][0]-2 % MOD;

       // LL ans = (mx * d% MOD + mxx*c% MOD + sum% MOD)%MOD;

        //cout<<c<<" "<<d<<endl;

        cout<<ans<<endl;

	}

}
```