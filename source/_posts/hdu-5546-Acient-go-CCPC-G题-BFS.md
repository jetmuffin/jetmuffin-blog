title: hdu-5546-Acient-go-CCPC-G题-BFS
date: 2015-11-12 15:12:31
tags: [bfs,搜索]
categories: 算法

---
# hdu 5546 Acient go CCPC G题 BFS

CCPC(中国大学生程序设计竞赛）G题`Acient go`解题报告

<!-- more -->

## 题目链接

[http://acm.hdu.edu.cn/showproblem.php?pid=5546](http://acm.hdu.edu.cn/showproblem.php?pid=5546)

## 题意

两人下棋，给定了现在的棋盘状况，轮到当前玩家下棋，问是否能够下一步棋将对方棋子吃掉

## 分析

当时在赛场上的思路还是正确的，求每个联通块周围的`.`的个数，计算的方法用bfs。然而我们在另一个地方出现了分歧，是否要取掉现在棋盘上已经被吃掉的棋子？题目上写着

> When one of the player makes his move, check the opponent's first. After removing the dead opponent;s components, check with the player's components and remove the dead components.

然后我们就针对着这句话纠结了好久，要不要拿掉对方的死子，要不要拿掉自己的死子，孰先孰后？

然而这也不是重点，敲完代码交了好几发WA，一直找不到错误出在哪里，卡了很久很久。

赛后良哥才发现一个最重要的问题，bfs计算每个联通块周围的`.`时，有可能会出现一个`.`被计算多次，顿时醒悟。

回来后把代码重新敲了，成功得到一个`Accept`，也是感慨地不行。

错误情况：

```
. . x x .
. x o o x
. . . o x
. x o o x
. . x x .
```
若简单相加，3行3列处的`.`会重复累加导致错误，正确的计算方法是类似`floodfill`的算法把每个联通块标号，相同联通块只能访问同一个`.`一次，而不同的联通块可以同时访问一个`.`

## 代码

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;

char cb[20][20];
char s[9];
struct node{
    int x, y;
    node(){}
    node(int a,int b):x(a),y(b){}
    void print(){
        cout<<x<<" "<<y<<" "<<endl;
    }
};
queue <node>q;
int flag = 0;
int vis[20][20];
int colors[20][20];
int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};
int tot,color;
void bfs(int x, int y){
    node a = node(x, y);
    q.push(a);
    while(!q.empty()){
        node b = q.front();
        q.pop();
        int curx = b.x;
        int cury = b.y;
        vis[curx][cury] = 1;    
        for(int k = 0; k < 4; k++){
            int nextx = curx + dx[k];
            int nexty = cury + dy[k];
            if(cb[nextx][nexty] == '.'){
                if(colors[nextx][nexty] != color){
                    tot++;
                    colors[nextx][nexty] = color;
                }
            }else if(cb[nextx][nexty] == 'o'){
                if(!vis[nextx][nexty]){
                    q.push(node(nextx,nexty));    
                }
            }
        }    
    }
}

int judge(){
    flag = 0;
    memset(vis, 0, sizeof(vis));    
    memset(colors, 0, sizeof(colors));
    color = 0;
    for(int i = 1; i <= 9; i++){
        for(int j = 1; j <= 9; j++){
            if(cb[i][j] == 'o' && !vis[i][j]){
                tot = 0;
                color++;
                bfs(i, j);
                if(tot == 1){
                    return 1;
                }
            }
        }
    }
    return 0;
}
int main(){
    int cas = 0,T;
#ifndef ONLINE_JUDGE  
  freopen("in.txt","r",stdin);  
#endif // ONLINE_JUDGE 
    scanf("%d",&T);
    while(T--){
        cas++;
        for(int i = 1; i <= 9; i++){
            scanf("%s", s);
            for(int j = 1; j <= 9; j++){
                cb[i][j] = s[j-1];
            }
        }
        for(int i = 0; i < 11; i++){
            cb[i][0] = '-';
            cb[0][i] = '-';
            cb[i][10] = '-';
            cb[10][i] = '-';
        }
        if(judge()){
            printf("Case #%d: Can kill in one move!!!\n", cas);
        }else{
            printf("Case #%d: Can not kill in one move!!!\n",cas);
        }
    }
}
```
