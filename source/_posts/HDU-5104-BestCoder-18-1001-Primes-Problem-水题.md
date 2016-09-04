title: 'HDU 5104 BestCoder #18 1001 Primes Problem 水题'
tags: [水题,素数]
categories: 算法题解
date: 2014-11-19 21:39:37
---

<span style="white-space:pre"></span>题意为对于每个n，求有多少种组合(a,b,c),a<=b<=c，a、b、c都是素数

<span style="white-space:pre"></span>事先打个1-10000的素数表，然后穷举a和b判断c是否为素数即可

<!--more-->

<span style="white-space:pre"></span>代码：

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

//#define mod 1000000007

#define eps 1e-8

#define MP make_pair

#define REP(i,a,b) for(int i = a; i < b; ++i)

#define RREP(i,a,b) for(int i = b; i > a ; --i)

#define RE freopen("in.txt","r",stdin)

//#define WE freopen("out.txt","w",stdout) 

const int maxn = 10000;

int notprime[maxn+1];

int prime[maxn+1];

int cnt = 0; 

void init()

{

    memset(notprime,0,sizeof(prime));

    notprime[0] = notprime[1] = true;

    for(int i = 2; i < maxn; i++)

    {

        if(!notprime[i])

        {

            if(i > maxn / i) continue;

            for(int j = i * i; j < maxn; j+=i)

                notprime[j] = 1;

        } 

    } 

} 

void getPrime()

{

    for(int i = 2; i < maxn; i++)

    {

        if(!notprime[i])

        {

            prime[cnt++] = i;

        }

    } 

}

int main()

{

    init();

    getPrime();

    int n;

    while(cin>>n)

    {

        int ans = 0;

        for(int i = 0;prime[i] <= n / 3;i++)

            for(int j = i ;n-prime[i]-prime[j]>=prime[j];j++)

            {

                int tmp = n - prime[i] - prime[j];

                if(!notprime[tmp])

                {

                    ans++;

                }

            }

        cout<<ans<<endl;

    }

}
```