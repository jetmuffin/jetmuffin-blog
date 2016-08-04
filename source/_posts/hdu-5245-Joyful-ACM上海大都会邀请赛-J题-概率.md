title: hdu 5245 Joyful ACM上海大都会邀请赛 J题 概率
tags: [hdu,邀请赛,概率,期望]
categories: 算法
date: 2015-06-06 23:47:43
---

**题目：**[http://acm.hdu.edu.cn/showproblem.php?pid=5245](http://acm.hdu.edu.cn/showproblem.php?pid=5245)

<!--more-->

**题意：**给定一个n行m列的方阵，每次选2个点(x1,y1),(x2,y2)，由这两个点为左上角和右下角可以确定一个矩形区域，将这个区域的格子涂黑；进行以上操作k次，问被涂黑的格子的数量的期望。

**分析：**针对每个格子(i,j)计算其被涂黑的概率，再将其相加则为涂黑格子数量期望，再进行k次操作即为答案。

每个格子，可以分为以下几个区域：

![](http://i1.tietuku.com/c190679b1de67e2e.png)

对于整个方阵，取一个点的种类共有m*n种，取两个点则有m<sup>2</sup>*n<sup>2</sup>种。

按后考虑将该格子(i,j),即图中的5号格子涂黑的种类数量。根据点1(x1,y1)的位置，可以确定点2(x2,y2)的位置分布，即可确定种类数。

*   若(x1,y1)在区域1,则(x2,y2)必然在区域5,6,7,8，种数(i-1)*(j-1)*(n-i+1)*(m-j+1);
*   若(x1,y1)在区域2,则(x2,y2)必然在区域4,5,6,7,8,9，种数(i-1)*1*m*(n-i+1);
*   若(x1,y1)在区域3,则(x2,y2)必然在区域4,5,7,8，种数(i-1)*(m-j)*(n-i+1)*j;
*   若(x1,y1)在区域4,则(x2,y2)必然在区域2,3,5,6,8,9，种数1*(j-1)*n*(m-j+1);
*   若(x1,y1)在区域5,则(x2,y2)必然在区域1,2,3,4,5,6,7,8,9，种数m*n*1*1;
*   若(x1,y1)在区域6,则(x2,y2)必然在区域1,2,4,5,7,8，种数(m-j)*1*n*j;
*   若(x1,y1)在区域7,则(x2,y2)必然在区域2,3,5,6，种数i*(m-j+1)*(n-i)*(j-1);
*   若(x1,y1)在区域8,则(x2,y2)必然在区域1,2,3,4,5,6，种数(n-i)*1*m*i;
*   若(x1,y1)在区域9,则(x2,y2)必然在区域1,2,4,5，种数(n-i)*(m-j)*i*j;

最后将种数相加再除以总数即为概率，对每个点的概率相加则为总该率，然后做k次就是答案了。

**代码：**

`#include <iostream>

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

int main()

{

    int t;

    cin>>t;

    int cnt = 0;

    while(t--){

        cnt++;

        int k;

        double n,m;

        cin>>n>>m>>k;

        double ans = 0;

        for(int i = 1; i <= n; i++ )

            for(int j = 1; j <= m; j++){

                double tot = 0.0;

                tot += (i-1)*(j-1)*(n-i+1)*(m-j+1);

                tot += (i-1)*1*m*(n-i+1);

                tot += (i-1)*(m-j)*(n-i+1)*j;

                tot += 1*(j-1)*n*(m-j+1);

                tot += m*n*1*1;

                tot += (m-j)*1*n*j;

                tot += i*(m-j+1)*(n-i)*(j-1);

                tot += (n-i)*1*m*i;

                tot += (n-i)*(m-j)*i*j;

                tot = tot /n/n/m/m; 

                ans += 1 - pow(1-tot,k);

            }

        printf("Case #%d: %d\n",cnt,int(ans+0.5));

    }

}`