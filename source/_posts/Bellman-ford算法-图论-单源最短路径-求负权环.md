title: Bellman-ford算法 图论 单源最短路径 求负权环
tags: [bellman-ford,图论]
categories: 算法
date: 2015-09-04 11:25:00
---

## 概念

Bellman-ford算法可以用来计算图中的单源最短路径。同时该算法可以处理边权为负的情况（Dijkstra算法无法处理负权），同时可以判断图中是否含有负权环。

设dis[v]为从源点s到v的最短路径长度。对于任意与v相连的顶点u，dis[v]可以满足三角不等式，即`dis[v]<=dis[u]+w(u,v)`，根据该性质可以设置松弛操作。
<!-- more -->
松弛一条边（u,v)：

1. 判断通过u对v的最短路径是否能够进行改进

2. 若可以，更新d[v]

伪代码：
```
    松弛（u,v,w): 
        if(d[v]>d[u]+w(u,v))
            d[v]=d[u]+w(u,v)
```

## 性质

1.  如果完成`|V|-1`轮操作后还能进行松弛操作（即仍然存在`d[v]>d[u]+w(u,v)`），那么图中有负权环
2.  若不存在负权环，经过`|V|-1`操作，min_dis[v] = dis[v]

## 算法

*   输入：图G（edge数组）和起点s
*   输出：s到每个点的最短路径，以及图G是否包含负权环
*   算法流程：

1.  初始化dis数组, `dis[s] = 0, dis[i](1<=i<=n, i!=s) = INF`
2.  枚举每一条边，进行松弛操作
3.  重复2操作`|V|-1`次
4.  枚举每一条边，看是否还能进行松弛操作
*   算法复杂度： `O(|V||E|)`

## 应用

### hdu 2544

求单源最短路径，直接使用即可，代码：

```cpp
#include <iostream>
#include <cstdio>
using namespace std;

#define INF 0x3f3f3f3f  
typedef struct Edge{
    int u,v;
    int w;
}Edge;

Edge edge[10010];
int dis[110];

bool Bellman_Ford(int n, int m){
    for(int i = 1; i <= n; i++){
        dis[i] = INF;
    }
    dis[1] = 0;
    for(int i = 2; i <= n; i++)
        for(int j = 1; j <= m; j++){

            if( dis[edge[j].u] > dis[edge[j].v] + edge[j].w)
                dis[edge[j].u] = dis[edge[j].v] + edge[j].w;
            
            if( dis[edge[j].v] > dis[edge[j].u] + edge[j].w)
                dis[edge[j].v] = dis[edge[j].u] + edge[j].w;
                
        }
}

int main(){
    int n,m;
    while(scanf("%d%d",&n,&m),m+n){
        for(int i = 1; i <= m; i++)
            scanf("%d%d%d",&edge[i].u,&edge[i].v,&edge[i].w);
        Bellman_Ford(n,m);
        printf("%d\n",dis[n]);
    }
}
```


### 判断负权环

只要在`|V|-1`次循环结束以后再枚举边即可

```cpp
bool Bellman_Ford(int n, int m){
    for(int i = 1; i <= n; i++){
        dis[i] = INF;
    }
    dis[1] = 0;
    for(int i = 2; i <= n; i++)
        for(int j = 1; j <= m; j++){

            if( dis[edge[j].u] > dis[edge[j].v] + edge[j].w)
                dis[edge[j].u] = dis[edge[j].v] + edge[j].w;
            
            if( dis[edge[j].v] > dis[edge[j].u] + edge[j].w)
                dis[edge[j].v] = dis[edge[j].u] + edge[j].w;
                
        }
        for(int j = 1; j <= m; j++){
            if( dis[edge[j].u] > dis[edge[j].v] + edge[j].w || dis[edge[j].v] > dis[edge[j].u] + edge[j].w)
            return 1;
        return 0;
}
```
 