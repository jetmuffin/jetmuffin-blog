title: 除法取模（逆元求法）总结

date: 2017-03-15 18:49:31

tags: [逆元,数论]

categories: 算法分析

---

最近刷的笔试题和以前打 ACM 中经常遇到求某个公式，最后的结果需要对某个数取模。我们都知道除法无法直接取模，即  $\frac{a}{b} \bmod c \neq \frac{a \bmod c}{b \bmod c} \bmod c$（当然，$ a \mid b $）。因此需要对 $b$ 求逆元，即 $\frac{a}{b} \bmod c = (a \bmod c)*(b^{'} \bmod c) \bmod c $ 。这里总结一下求逆元的方法。

<!--more-->

## 定义

对于正整数 $a$ 和 $m$，如果有 $ax \equiv 1 \pmod m$，那么把同余方程组里 $x$ 的最小正整数解称为 $a$ 模 $m$ 的逆元。

## 求法

### 费马小定理

**前提：$a$ 与 $m$ 互质**。

一般情况下题目会给小于 `int` 的最大素数 `1e9+7`，那么费马小定力就可以用了。

$$ a^{m-1} \equiv 1 \pmod m \Rightarrow a \cdot a^{m-2} \equiv 1 \pmod m \Rightarrow a^{m-2} \equiv \frac{1}{a} \pmod m $$

那么逆元即为 $a^{m-2} \bmod m$

```cpp
const int mod = 1000000007;
long long quick_pow(long long a, long long b) {
  long long res = 1;
  while(b) {
    if(b & 1) {
      res = a * res % mod;
    }
    a = a * a % mod;
    a >>= 1;
  }
  return res;
}
long long inv(long long x) {
  return quick_pow(a, mod - 2) % mod;
}
```

### 扩展欧几里得

**前提：$a$ 与 $m$ 互质**

由逆元定义可知，给定模数 $m$，求 $a$ 的逆相当于求解 $ax \equiv 1 \pmod m$。方程可以转化为 $ax-my=1$。那么当 $gcd(a, m) = 1$ 时，可以通过扩展欧几里得算法算出解，再将算出的特解转化到 $[0,m−1]$ 即可。

```cpp
void ext_gcd(long long a, long long b, long long &x, long long &y) {
  if(b == 0) {
    x = 1;
    y = 0;
  } else {
    ext_gcd(b, a % b, x, y);
    long long t;
    t = x;
    x = y;
    y = t - a / b * y;
  }
}
long long inv(long long x) {
  long long now, t;
  ext_gcd(x, mod, now, t);
  return (now % mod + mod) % mod;
}
```

### 欧拉函数

TBD