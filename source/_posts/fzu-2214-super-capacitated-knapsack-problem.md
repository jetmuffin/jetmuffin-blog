title: fzu 2214 super capacitated knapsack problem
date: 2017-05-23 20:38:49
tags: [背包, dp]
categories: 算法题解
---

## 题目

[Problem 2214 Knapsack problem](http://acm.fzu.edu.cn/problem.php?pid=2214)

<!-- more -->

## 分析

题目的意思和普通的 01 背包一模一样，不过在背包的容量上非常大，我们知道传统的 01 背包复杂度为 $o(wn)$，而在题目中 w 的范围达到了 $10^9$，因此传统 01 背包肯定是会超时的。

这道题比较有意思的地方是，思路是从反方向思考，颠倒了 01 背包的方向。

传统 01 背包里，递推公式为 $dp_j=max(dp_j, dp_{j-w_i}+v_i)$，而在超大背包中，我们可以先算出物品的总价值 $v_{sum}$，再将价值作为物品，物品作为价值，即计算得到价值为 $j$ 时的最小容量 $dp_j$。递推公式为 $dp_j=min(dp_j, dp_{j-v_i}+w_i)$。

最后，遍历 $j$，找到第一个 $dp_j < w_{total}$ 就是我们需要求的最大价值了。

这样总体复杂度为 $o(v_{sum}n)$

## 代码

```cpp
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
typedef long long ll;
const int maxn = 5100;
const ll inf = 1000000100;
ll w[maxn], v[maxn], dp[maxn];
int main() {
    int t, n;
    ll p;
    // scanf("%d", &t);
    cin>>t;
    while(t--) {
        // scanf("%d%lld", &n, &p);
        cin>>n>>p;

        ll sum = 0;
        for(int i = 0; i < n; i++) {
            // scanf("%lld%lld", &w[i], &v[i]);
            cin>>w[i]>>v[i];
            sum += v[i];
        }
        memset(dp , 0x3f3f3f3f , sizeof(dp));
        dp[0] = 0;
        for(int i = 0; i < n; i++) {
            for(int j = sum; j >= v[i]; j--) {
                dp[j] = min(dp[j], dp[j - v[i]] + w[i]);
            }
        }

        for(int i = sum; i >= 0; i--) {
            if(dp[i] <= p) {
                // printf("%d\n", i);
                cout<<i<<endl;
                break;
            }
        }
    }
}

```