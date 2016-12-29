title: 求多个数的最小公倍数lcm
tags: [数论,最小公倍数]
categories: 算法分析
date: 2015-09-04 10:21:00
---

## 欧几里得法

由公式`lcm(a,b) = a*b/gcd(a,b)`可以得到两个数的lcm，同时多个数的最小公倍数可以先求前两个数的最小公倍数，再和第三个数求最小公倍数，以此类推。
<!-- more -->
### 复杂度

`O(nlog(n))`

### 代码

```cpp
int gcd(int a, int b){
    return b == 0? a : gcd(b, a%b); 
}
int lcm(int x, int y){
    return x*y/gcd(x,y);
} 
int lcm2(int n){
    int ret = 1;
    for(int i = 0; i < n; i++){
        ret = lcm(ret, a[i]);
    }
    return ret;
}
```

## 分解质因数法

将每个数分解成质因数p的幂次的乘积的形式，则每个质因数在lcm中出现的次数为在每个a[i]中出现次数的最大值，统计每个质因数p出现的次数，最后做一次乘积即可。

```latex
lcm = p_{1}^{b_{1}}\times p_{2}^{b_{2}}\times p_{3}^{b_{3}}\times ... p_{k}^{b_{k}}
```

### 复杂度

`O(nlog(n))`

### 代码

```cpp
int lcm1(int n){
    for(int i = 0 ; i < n; i++){
        for(int j = 2; j * j <= a[i]; j++){
            int cnt = 0;
            while(a[i] % j == 0){
                cnt ++;
                a[i] /= j;
            }
            b[j] = max(b[j], cnt);
        }
        if(a[i] > 1){
            b[a[i]] = max(b[a[i]], 1);
        }
    }
    int ret = 1;
    for(int i = 2; i < 10000; i++){
        for(int j = 1; j <= b[i]; j++)
            ret = ret * i % mod;
    }
    return ret;
}
```
  