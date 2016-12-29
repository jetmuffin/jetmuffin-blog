title: SPFA 单源最短路径 队列优化版Bellman-ford
tags: [图论,spfa,最短路径]
categories: 算法分析
date: 2015-09-04 17:34:00
---

## 思路

SPFA是队列优化版的Bellman-ford，核心思想和Bellman-ford相同，不停松弛节点。但是在Bellman-ford中需要对所有边的相应点都进行松弛，进行了过多的不必要操作。而SPFA则是将松弛过的点加入队列维护，使得被松弛的点尽量少。这样使得最坏情况下SPFA复杂度等于Bellman-ford，一般情况下远大于Bellman-ford

<!-- more -->

## 实现

*   复杂度：最坏情况O(|V|+|E|)
*   输入：n 图的顶点数（|V|）
    src 源点
    g 邻接表
*   输出：dis dis[i]表示src到点i的最短距离

## 应用

### hdu2544
```cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

#define INF 0x7FFFFFFF   
const int maxn = 150;
vector <pair<int, int> > g[maxn];

int dis[maxn];
int vis[maxn];


void spfa(int n){
    for(int i = 1 ; i <= n; i++){
        dis[i] = INF;
    }
    memset(vis, 0, sizeof(vis));
    queue<int> q;
    dis[1] = 0;
    vis[1] = 1;
    q.push(1);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        vis[u] = 0;
        for(int i = 0; i < g[u].size(); i++){
            int v = g[u][i].first;
            int w = g[u][i].second;
            if(dis[u] + w < dis[v]){
                dis[v] = dis[u] + w;
                if(vis[v] == 0){
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
    }
}

int main(){
    int n,m;
    while(scanf("%d%d",&n,&m),m+n){
        for(int i = 0; i < n; i++)
            g[i].clear();
        for(int i = 1; i <= m; i++){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            g[u].push_back(make_pair(v,w));
            g[v].push_back(make_pair(u,w));
        }
        spfa(n);
        printf("%d\n",dis[n]);
    }
}
```
    