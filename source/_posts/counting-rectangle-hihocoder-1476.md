title: 矩形计数 hihocoder 1476
date: 2017-03-05 20:18:11
categories: 算法题解
tags: [容斥原理]
---
## 题目

题目地址 [http://hihocoder.com/problemset/problem/1476](http://hihocoder.com/problemset/problem/1476)


![图1](https://media.hihocoder.com//problem_images/20170305/14886790334060.png)

<!-- more -->

如图所示， 给定的矩形中有 K 个单位正方形是黑色的，其余是白色的，求多少个不同的子矩形是完全由白色的正方形组成的。

## 思路

直接算由白色正方形组成的不好计数，需要反向通过减去包含黑色方块的计算更方便。但是减去黑色方块时需要用**容斥原理**。即总矩形数减去包含 1 个黑色方格的矩形数，加上包含 2 个黑色方格的矩形数，减去包含 3 个黑色方格的矩形数……依次类推。

可以想到的是，对于 n * m 大小的矩形，包含的所有子矩形的个数的 $\binom{n}{2} * \binom{m}{2}$。

而包含 t 个黑色方格的矩形数，可以首先处理出所有这些黑色方格的范围的四条线，$x_1$，$x_2$， $y_1$，$y_2$，那么如果矩形要包括所有的黑格，它的上横线必然从 $[0, x_1]$ 取，下横线从 $[x_2, n]$ 取，那么横线的取法有 $\binom{x_1}{1} * \binom{n - x_2 + 1}{1}$ 种，同理可得纵线取值，最后可以得到包含黑色方格的取法一共有 $x_1*(n-x_2+1)*y_1*(m-y_2+1)$ 种。

最后通过容斥原理统计一遍即可。

![图2](http://cdn.jetmuffin.com/posts/hihocoder_1476.png)

## 代码

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
#define ll long long
const int maxn = 1010;
const int inf = 0x3f3f3f;
ll n, m, k;
ll r[maxn], c[maxn];
int main() {
	while(~scanf("%lld%lld%lld", &n, &m, &k)) {
		for(int i = 0; i < k; i++) {
			scanf("%lld%lld", &r[i], &c[i]);
		}
		ll ans = n * (n + 1) / 2 * m * (m + 1) / 2;
		for(int i = 1; i < (1 << k); i++) {
			int num = __builtin_popcount(i);
			int x1 = inf, x2 = 0, y1 = inf, y2 = 0;
			for(int j = 0; j < k; j++) {
				if(i & (1 << j)) {
					if(x1 > r[j]) x1 = r[j];
					if(x2 < r[j]) x2 = r[j];
					if(y1 > c[j]) y1 = c[j];
					if(y2 < c[j]) y2 = c[j];
				}
			}
			if(num % 2 == 0) ans += x1 * y1 * (n - x2 + 1) * (m - y2 + 1);
			else ans -= x1 * y1 * (n - x2 + 1) * (m - y2 + 1);
		}
		printf("%lld\n", ans);
	}
}
```