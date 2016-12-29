title: 最少费用购物问题，裸DP，动态规划
tags: [dp]
categories: 算法题解
date: 2014-10-30 07:05:55
---

算法导论结课题目。

题目很长，大义是给出每个商品的编码和费用，再给出某些商品的组合价，求买了所需商品的最少费用。

<!--more-->

样例输入：

2

7  3  2

8  2  5

2

1  7  3  5

2  7  1  8  2  10

样例输出：

14

样例解释：

第一行2代表有2个商品，之后两行中，第一个数字是商品编码，第二个数字是商品所需购买数量，第三个数字是商品单价。

第二行2代表优惠组合数，第一个数字代表有多少商品的组合，之后每一对数字代表组合的商品的编码和数量，该行最后一个数字代表优惠组合优惠价格。

题目看起来很复杂，读起来更复杂- -。

第一眼看上去很像背包，刚开始试着用多重背包的思想去做，发现二维完全解决不了（因为需要各个商品的数量作为状态）。仔细看发现商品数量最大为5，果断使用普通的裸DP，不过维数较多，有5维，也就意味着有至少5个for循环，蛋疼至极。打算试试状态压缩，发现不好办，最后还是老老实实编码了。

注意点：优惠商品数量不能大于需求数量，同时注意商品编码和输入的i、j的关系要处理好。

PS：写完后雪林哥给了一组数据，我的程序跪了，发现就是商品编码没处理好，ORZ～

分享该数据：

输入：

5

1 1 2

2 1 3

3 1 4 

4 1 5

5 1 6

6

1 2 1 3 

2 1 1 2 1 5

5 1 1 2 1 3 1 4 1 5 1 3

3 1 1 3 1 4 1 1

2 2 1 5 1 1

1 1 1 3

输出：2

代码如下：

```cpp
#include <iostream>  
#include <cstdio>  
#include <algorithm>  
#include <cstring>  
#include <queue>  
#include <set>  
#include <map>  
#include <time.h>  
using namespace std;  

int sale[1000][6] = {0};    //分别表示每个优惠中每个商品数量   
int saleprice[1000] = {0};  //优惠总价   
int salelength[1000] = {0}; //优惠总共有几个商品   
int salenumber[1000][1000] = {0};   //优惠商品的ID   
int good[6][4] = {0};       //1 -> number   2 -> price  3 -> last num  
int num[1000];              //商品ID   
int dp[6][6][6][6][6];    
int n,m;  

void input() {  
    cin>>n;
    for(int i = 1; i  <= n; i++) {  
        cin>>good[i][1]>>good[i][3]>>good[i][2];  
        num[i] = good[i][1];  
    }  
    
    cin>>m;  
    
    for(int i = 1; i <= m; i++) {  
        cin>>salelength[i];  
        for(int j = 1; j <= salelength[i]; j++) {  
            cin>>salenumber[i][j];
            cin>>sale[i][salenumber[i][j]];  
        }  
        cin>>saleprice[i];  
    }  
}  

void output() {  
    for(int i = 1; i  <= n; i++)  
        cout<<"goodnum: "<<good[i][1]<<" goodprice: "<<good[i][2]<<" goodlast: "<<good[i][3]<<endl;  
    for(int i = 1; i  <= m; i++) {  
        //cout<<salelength[i]<<endl;  
        cout<<"sale"<<i<<" : ";  
        for(int j = 1; j <= salelength[i]; j++)  
                cout<<"num: "<<salenumber[i][j]<<" count: "<<sale[i][salenumber[i][j]]<<" ";  
        cout<<endl;  
       cout<<" price: "<<saleprice[i]<<endl;  
    }  
}  

int main() {
    //freopen("in2","r",stdin);  
    input();  
    // output();  
    dp[0][0][0][0][0] = 0;  
    for(int i = 0; i <= good[1][3]; i++)  
        for(int j= 0; j <= good[2][3]; j++)  
            for(int k = 0; k <= good[3][3] ;k++)  
                for(int l = 0; l <= good[4][3]; l++)  
                    for(int p = 0; p <= good[5][3]; p++) {
                        int minx = i * good[1][2] + j * good[2][2] + k * good[3][2] + l * good[4][2] + p * good[5][2];  
                        for(int q = 1; q <= m; q++) {
                            if(i -sale[q][num[1]]<0 || i - sale[q][num[2]]<0 ||   i-sale[q][num[3]]<0 || i-sale[q][num[4]]<0 ||i-sale[q][num[5]]<0)
                                continue;  
                            int t = dp[i - sale[q][num[1]]][j - sale[q][num[2]]][k - sale[q][num[3]]][l - sale[q][num[4]]][p - sale[q][num[5]]] + saleprice[q];  
                            if(t < minx) minx = t;  
                        }
                        dp[i][j][k][l][p] = minx;  
                    }  

                    cout<<dp[good[1][3]][good[2][3]][good[3][3]][good[4][3]][good[5][3]]<<endl;  
    return 0;  
}
```
