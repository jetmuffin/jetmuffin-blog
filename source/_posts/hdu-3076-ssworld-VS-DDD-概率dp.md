title: hdu 3076 ssworld VS DDD 概率dp
tags: [概率dp]
categories: 算法题解
date: 2014-10-14 18:24:35
---

题意应该很明显，甩骰子决定谁扣血，输的扣血，赢的不变，平局双方都不变。

解题时有好几个地方想错了：

1. 原本以为dp[i][j]表示当前A和B的血量，结果想反了，整个思路貌似有点偏

2. 以为平局的概率也要加入，事实上也是错误1引起的，修改为dp[i][j]表示A和B赢的次数就好理解了，简化问题，去掉平局，p1 = win/(win+lose) ; p2 = lose/(win+lose)

3. 最后的答案不是dp数组中的一个，而是A赢了hp2次的概率，就是dp[hp2-1][j(j从0到hp1-1求和)]*win 

解决了这几个问题，结果提交还是WA，后来看了discuss才发现数据错了，两人血量反了，所以把其中的i和j互换。

<!--more-->

代码：

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

#define WE freopen("out.txt","w",stdout)   

#define N 2001  

double a[7],b[7];   

double dp[N][N];  //dp[i][j]表示A赢了i局，B赢了j局，最后求A赢了hp2局的概率   

int main()  

{  

    RE;  

    int hp1,hp2;  

    while(cin>>hp1>>hp2)  

    {  

        memset(dp,0,sizeof(dp));  

        double win,lose,draw;  

        win = lose = draw = 0;  

        for(int i=1;i<=6;i++)  

            cin>>a[i];  

        for(int i=1;i<=6;i++)  

            cin>>b[i];  

        for(int i=2;i<=6;i++)  

            for(int j=1;j<i;j++)  

                win += a[i]*b[j];  

        for(int i=1;i<=5;i++)  

            for(int j=i+1;j<=6;j++)  

                lose += a[i]*b[j];  

        draw = 1-win-lose;  

        double win_p = (win)/(1-draw);  

        double lose_p = lose/(1-draw);                        

        dp[0][0] = 1;   

        for(int j=0;j<=hp1-1;j++)  

            for(int i=0;i<=hp2;i++)  

            {  

                if(i+j>0)  

                {  

                    if(i>0)  

                            dp[i][j] += dp[i-1][j]*lose_p;                    

                    if(j>0)  

                        dp[i][j] += dp[i][j-1]*win_p;         

                }  

            }  

        //cout<<dp[hp2-1][hp1-1]<<endl;   

        double ans = 0;  

        for(int i=0;i<=hp2-1;i++)  

            ans += dp[i][hp1-1] * win_p;  

        if(ans>1) ans=1;  

        printf("%.6f\n",ans);  

    }  

}
```