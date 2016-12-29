title: 'Hdu 5178 BestCoder #31 1001 Pairs 解题报告'
tags: [二分]
categories: 算法题解
date: 2015-03-06 12:36:36
---

题意简单，找到线段上距离小于k的点对的个数。

解法为，先排序所有点，然后遍历每个点x[a]，二分刚好小于b=x[a]+k的所在点下标，表示a和a+1～b之间的点都为符合点。然后累加b-a即可。

用lower_bound会WA，要用upper_bound或者自己手写二分，问题出在当x[b]=x[a]+k时的区别。

<!--more-->

代码：

```cpp
#include <iostream>

#include <algorithm>

#include <cstring>

#include <cstdio>

using namespace std;

#define LL long long

#define MAXN 100005

int x[MAXN];

int main()

{

  int T;

  cin>>T;

  while(T--)

  {

    int n,k;

    scanf("%d%d",&n,&k);

    for(int i = 0 ; i < n ; i++)

    {

      scanf("%d",&x[i]);

    }

    sort(x,x+n);

    LL ans = 0;

    for(int i = 0; i < n; i++)

    {

      int b = upper_bound(x + i + 1, x + n, x[i] + k) - x - i - 1;

      // cout<<b<<endl;

      ans += b; 

    }

    cout<<ans<<endl;

  }

}
```