title: hdu 5241 Friends ACM上海大都会邀请赛 F题
tags: [数论,邀请赛,结论题]
categories: 算法题解
date: 2015-06-06 23:16:31
---

**题目：**[http://acm.hdu.edu.cn/showproblem.php?pid=5241](http://acm.hdu.edu.cn/showproblem.php?pid=5241)

<!--more-->

**题意：**Mike会n种语言，而从Alice到Irene会的语言满足一系列的子集关系，问题为给定第一个n，所有的情况总数数量。

**分析：**由于各种语言互相独立，所以最后的答案肯定是一个数的n次方，即k<sup>n</sup>，而对于这个k可以用暴力的方法解出来，即当n=1的时候的数量，以下为解法：

各个小伙伴的语言关系如下图，其中有向箭头表示语言"≥"。

![](http://i1.tietuku.com/e3384532b1bd57d2.png)

可以从图中得出的答案是，以a,b,c,d,e,f,g,h,i表示Alice, Bob, Carol, Dave, Eve, Frank, Gloria, Henry and Irene所会语言种数（0或者1）的可能性数量，则满足以下条件：
<pre style="padding-left: 30px;">a = n
d = n
e = min(a,d)
g = d
b = min(a,g)
c = min(b,e)
f = min(g,e)
h = g
i = h
</pre>

再根据以上条件暴力解的k的大小为32：

```cpp
#include <iostream>

#include <algorithm>

using namespace std;

int main(){

  int n = 1;

  int ans = 0;

  for(int a = 0; a <= n;a++)

    for(int d = 0; d <=n ; d++)

      for(int e = 0;e <= min(a,d);e++)

        for(int g = 0; g<= d;g++)

          for(int b = 0; b<= min(a,g);b++)

            for(int c = 0; c<= min(b,e);c++)

              for(int f = 0; f<= min(g,e);f++)

                for(int h = 0 ; h<=g;h++)

                  for(int i = 0; i <= h ;i++)

                    ans++;

  cout<<ans<<endl;

}`

然后直接上java的大数就行了！

`import java.math.BigInteger;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        int t = sc.nextInt();

        int cnt = 0;

        while(t > 0){

            t--;

            cnt++;

            int n = sc.nextInt();

            BigInteger base = BigInteger.valueOf(32);

            BigInteger ans = BigInteger.valueOf(1);

            for(int i = 0 ; i < n; i++){

                ans = ans.multiply(base);

            }

            System.out.println("Case #" + cnt +": "+ans);

        }

    }

}
```