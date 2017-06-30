title: hihocoder 1489 legendary items 微软2017笔试第一场
date: 2017-04-09 19:39:17
tags: [概率dp]
categories: 算法题解
---

## 题目

题目地址 [http://hihocoder.com/problemset/problem/1489](http://hihocoder.com/problemset/problem/1489)


题目大意是，小 Hi 按一定的初始概率 `P` 夺宝（成功概率为 `P`，失败为 `1-P`，如果在某个关卡失败了，那么在下一个关卡他成功的概率会上升 `Q`；如果在某个关卡成功了，那么他在下个关卡成功的概率会变为 `⌊P/(2I)⌋`（`I` 为当前已经拿到的宝物数量）。问小 Hi 得到 N 个宝物需要闯关多少次。

<!--more-->

## 思路

笔试的时候读题读了半天才弄懂题意，拿到手第一感觉是概率 dp，推一个一维公式推了好长一段时间，然后期间又转战另一个二维的递推方程，结果方程有了，实现的时候写了半天，最后没写出这道题。赛后把两个思路的代码都写完了，这里记录一下这两个思路。

### 方法 1

**递推方程：**

$$ dp[i, j] = (i / 100) * ((dp[next_{i}, j+1] + 1) + ((100 - i) / 100 * (dp[i + q] + 1) $$

其中 $dp[i,j]$ 表示已经取了 $j$ 个宝物，当前状态闯关成功概率为 $i$ 时还需要 $dp[i,j]$ 步才能取完所有 N 个宝物。

那么公式中，前半部分为以$i/100$ 的概率闯关成功，那么还需要的步数为现在走的这一步加上下一个状态 $dp[next_{i}, j+1]$ 的剩余步数。其中 $next_{i}$ 为闯关成功后刷新的下一个状态的成功概率，即 `⌊P/(2I)⌋`。

后半部分为以 $(100 - i) / 100$ 概率闯关失败，那么还需要现在走的这一步加上下一个状态 $dp[i + q]$ 的剩余步数。

那么按这个思路的代码如下：

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
using namespace std;
int p, q, n;
const int maxn = 1000001;
double dp[101][maxn];
int nxt[maxn];
void solve() {
    nxt[0] = p;
    int nxti, nxtii;

    for(int j = n-1; j >= 0; j--) {
        for(int i = 100 ; i >= 0; i--) {
            nxti = nxt[j+1];
            nxtii = i + q >= 100? 100 : i+q;

            dp[i][j] = (i / 100.0) * (dp[nxti][j+1] + 1.0) + (100 - i) / 100.0 * (dp[nxtii][j] + 1.0);
            // printf("(%d %d)->(%lf %lf %lf %d) ;\n", i, j, (i / 100.0) * (dp[nxti][j+1] + 1), (100 - i) / 100.0 * (dp[min(i + q, 100)][j] + 1), dp[i][j], nxti);
        }
    }

}
void pre() {
    nxt[0] = p;
    for(int i = 1; i <= n; i++) {
        nxt[i] = nxt[i - 1] / 2;
    }
}
int main() {
    scanf("%d%d%d", &p, &q, &n);
    pre();
    solve();
    printf("%.2lf\n", dp[p][0]);
}
```

算法的复杂度为 `O(100N)`，在 `1 ≤ N ≤ 1e6` 的情况下时间非常吃紧，后来交的几发提交都 T 了。

实际上从递推方程中可以发现的是，有许多情况下的概率 `i` 是不会出现的，那么也就没有必要遍历去求。在这个基础上用 dfs 加记忆化去求这个方程可能就可以减少计算量。

### 方法 2

**递推方程：**

$$ dp[i] = \sum_{j=0}^{(100-s)/q} (s * j * \prod_{k=0}^{j-1}( 1 - (s+k*q))) $$

其中 $dp[i]$ 表示从取得 $i-1$ 个宝物到取得 $i$ 个物品需要的步数。严格意义上讲这并不是一个 dp，因为事实上每个 $dp[i]$ 是独立的。

公式中 $s$ 表示手上有 $i-1$ 个物品时的闯关成功概率。

在手上有 $i-1$ 个物品的情况下，最多失败 $(100-s)/q$ 次。

而失败 $j$ 次的概率是可以计算的，例如起始闯关成功概率 $s$，失败3次，那么总概率为 $(1-s)*(1-s+q)*(1-s+2q)*(s+2q)$ 再乘上步数就是期望了。

最后答案就是 $\sum_{i=0}^n dp[i]$

根据这个思路写的代码如下：

```cpp
#include <iostream>
#include <cstring>
using namespace std;
const int maxn = 1000001;
double dp[maxn];
int nxt[maxn];
int p, q, n;
void pre() {
    nxt[0] = p;
    for(int i = 1; i <= n; i++) {
        nxt[i] = nxt[i - 1] / 2;
    }
}
void solve() {
    dp[0] = 0;
    for(int i = 1; i <= n; i++) {
        int cnt = (100 - nxt[i-1]) / q;
        if(cnt * q == 100 - nxt[i-1]) cnt--;
        cnt++;
        // printf("i(%d) cnt(%d)\n", i, cnt);
        for(int j = 0; j <= cnt; j++) {
            double t = 1.0;
            double s = nxt[i-1] / 100.0;
            for(int k = 0; k < j; k++) {
                t *= (1 - s);
                // printf("i(%d) j(%d) k(%d) %lf\n", i, j, k, 1 - s);
                s += q / 100.0;
            }
            t *= s > 1 ? 1 : s;
            t *= j + 1;
            // printf("s(%lf) t(%lf)\n", s, t);
            dp[i] += t;
        }
    }
}
int main() {
    while(~scanf("%d%d%d", &p, &q, &n)) {
        memset(dp, 0, sizeof(dp));
        pre();
        solve();
        double ans = 0;
        for(int i = 0; i <= n; i++) {
            ans += dp[i];
        }
        printf("%.2lf\n", ans);
    } 
}
```

这个思路的复杂度为 `O(100N/q)` 平均情况会比上个方法好。

## 后续

这场笔试雪崩，虽然还有一年，还是得多练。。