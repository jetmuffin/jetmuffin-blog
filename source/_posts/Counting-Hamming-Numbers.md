title: Counting Hamming Numbers
date: 2017-02-18 10:26:40
tags: [prime, 因数分解]
categories: 算法题解
---
[Hamming Number](http://oeis.org/A051037) 是一类特殊的数，他们的因数仅为2，3，5 中的任意组合。形式化地说即：

$$x = 2^i \times 3^j \times 5^k, i,j,k \geq 0, \forall x \in H$$

<!-- more -->

## 求解

若要求解 Hamming Number，第一反应是使用一个最小堆来维护整个数列，不断从堆里取出最小的数 `top`，再往堆里放入 `2*top`，`3*top`，`5*top`，同时对拿出来的数进行去重，整个计算的复杂度是 `O(logn)` 的。

大概的解决代码如下：

```cpp
long countHammingNumber(long n) {
  priority_queue<long, vector<long>, greater<long> > q;
  long peak = 1;
  q.push(2);
  q.push(3);
  q.push(5);
  for(int i = 1; i < n; i++) {
    peak = q.top();
    q.pop();
    if(!q.empty()) {
      while(q.top() == peak) {
        q.pop();
      }
    }
    q.push(peak * 2);
    q.push(peak * 3);
    q.push(peak * 5);
  }
}
```

通过这样处理，最小堆每次取出的数放进一个数组里则这个数组里就全是 Hamming Number 了。

然而这样的处理比较耗时，再仔细分析 Hamming Number 的由来可以想到：

x（Hamming Number） 的因数仅存在 2、3、5，那么只要 count 2、3、5 的个数即可了。列一个表可以表示为：

|n-th|5|3|2|value|
|---|---|---|---|--|
|1|0|0|0|1|
|2|0|0|1|2|
|3|0|1|0|3|
|4|0|0|2|4|
|5|1|0|0|5|
|6|0|1|1|6|
|7|0|0|3|8|
|...|...|...|...|...|

从表中可以看出 Hamming Number 的递推过程存在一定规律，可以用 dp 的思路去解决，大体可以表示成：

```
dp[i] = min(dp[j]*2, dp[k]*3, dp[l]*5)
```

即我们维护上表中的 `5、3、2` 因子为 `j、k、l`，那么当计算某个 `n-th` 即 `i` 时，`dp[i]` 就通过上式子递推得出。这样求解的话复杂度为 `O(n)`

代码大致如下：

```cpp
int countHammingNumber(int n) {
  int dp[1690];
  int j = 1, k = 1, l = 1;
  dp[1] = 1;
  for(int i = 2; i <= n; i++) {
    dp[i] = min(2 * dp[j], min(3 * dp[k], 5 * dp[l]));
    if(dp[i] == 2 * dp[j]) j++;
    if(dp[i] == 3 * dp[k]) k++;
    if(dp[i] == 5 * dp[l]) l++;
  }
  return dp[n];
}
```

## 题目

- [Ugly Number](https://leetcode.com/problems/ugly-number/?tab=Description)
- [Ugly Number II](https://leetcode.com/problems/ugly-number-ii/?tab=Description)


