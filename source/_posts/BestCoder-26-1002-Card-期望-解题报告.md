title: 'BestCoder #26 1002 Card  期望 解题报告'
tags: []
date: 2015-01-27 20:12:10
---

题目的意思是，从x张牌里抽1张牌，然后放回重复b次，Sj为每次抽出牌的积分之和，问Sj的期望。

一开始看这个题目的时候考虑用组合做，因为b最大为5，所以考虑Sj由5个数-1个数组成的情况，然后算概率，但是题目的a和T的范围太大，O(Tlog(a))都会T估计，这明摆着是要O(T)的节奏啊。。也就是说，给定一个x，一个b，就有公式可以推出答案。

然后从另一个方向考虑，如果从组合考虑，即使给定几个数被抽到，还得考虑顺序情况，相当复杂，于是可以考虑对于每个数，其实只有2种状态（选中OR未选中），根据这两种状态去求期望会方便很多。

每次抽牌，选中某一张牌的概率为1/x；

抽b次牌结束后，某张牌被抽中概率为p = 1-(1-1/x)^b，（转化为相反的事件再转化回来）。

因此抽中某一张牌对期望的贡献为 i * p ，因此总期望为 ∑(i=1~x) i * p ，即EX = (x + 1)*x* (1-(1-1/x)^b)。

<!--more-->

代码：

`#include <iostream>  

#include <cstdio> 

using namespace std;  

int cnt = 0;  

int main()  

{  

    int T;  

    cin>>T;  

    while(T--)  

    {  

        cnt++;  

        double x;  

        int b;  

        scanf("%lf%d",&x,&b);  

        double p = 1.0;  

        for(int i = 0; i < b; i++)  

            p *= (1 - 1/x);  

        double ans = (1 - p) * (x + 1) * x / 2;  

        printf("Case #%d: %.3f\n",cnt,ans);  

    }  

}`