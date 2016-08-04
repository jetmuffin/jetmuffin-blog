title: FZUOJ 1759 Super A^B mod C 指数循环节
date: 2015-12-08 20:38:44
tags: [数论,欧拉函数,循环节]
---

## 题目

[http://acm.fzu.edu.cn/problem.php?pid=1759](http://acm.fzu.edu.cn/problem.php?pid=1759)

## 题意

给定A，B和C的值，求$A^{B}modC的值，其中`1<=A,C<=1e10,1<=B<=10e1000000`

## 分析

今天刚刚学到一个内容，指数降幂公式，即$A^{B}modC=A^{Bmodphi(c)+phi(c)}modC$

用这个公式，可以很方便地对大指数问题进行降维处理。
<!-- more -->
## 代码

```cpp
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;
const int maxn = 1000005;
typedef long long ll;

char str[maxn];

int phi(int n){
  int res = n;
  for(int i = 2; i * i <= n; i++){
	if(n % i == 0){
	  res = res - res / i;
	  while(n % i == 0) n /= i;
	}
  }
  if(n > 1)
	res = res - res / n;
  return res;
}

ll multi(ll a, ll b, ll m){
	ll ans = 0;
	a %= m;
	while(b){
		if(b & 1){
			ans = (ans + a) % m;
			b--;
		}
		b >>= 1;
		a = (a + a) % m;
	}
	return ans;
}

ll quickmod(ll a, ll b, ll m){
	ll ans = 1;
	a %= m;
	while(b){
		if(b & 1){
			ans = multi(ans, a, m);
			b--;
		}
		b >>= 1;
		a = multi(a,a,m);
	}
	return ans;
}

int main(){
 	ll a,c;
 	while(~scanf("%I64d%s%I64d", &a, str, &c)){
 		ll len = strlen(str);
 		ll ans = 0;
 		ll p = phi(c);
 		if(len <= 15){
 			for(int i = 0 ; i < len; i++)
 				ans = ans * 10 + str[i] - '0';
 		}else{ // A^B%C = A^(B%phi(C)+phi(C))%C
 			for(int i = 0 ; i < len ; i++){
 				ans = ans * 10 + str[i] - '0';
 				ans %= p;
 			}
 			ans += p;
 		}
 		printf("%I64d\n", quickmod(a,ans,c));
 	}
}

```
