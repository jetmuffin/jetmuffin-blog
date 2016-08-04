title: Floyd-Warshall算法 多源最短路径
tags: [图论,floyd]
categories: 算法
date: 2015-09-05 11:36:00
---

## 算法思路

Floyd算法的原理即动态规划，令D(i,j)表示从i点到达j点的最短路径长度。那么从i点到达j点有两种可能：

1.  直接从i到达j不经过中间点k
2.  i经过中间点k到达j，那么需要比较D(i,k)+D(k,j)和D(i,j)的大小并且更新最小值

    我们遍历所有的点k,就可以得到从点i到j的最短路径了，而D数组不需要重新开辟，直接在邻接矩阵g上操作就行了

<!-- more -->
## 算法流程

伪代码：
```
    k: 1 ->; n
        i: 1 -> n
            j: 1 -> n
                g[i][j] = min(g[i][j],g[i][k]+g[k][j])
```

## 应用

### hdu 2544

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
#define INF 100000000  
const int maxn = 105;
int g[maxn][maxn];
int min(int a,int b){
    return a<b?a:b;
}

void floyd(int n ){
    for(int k = 1; k <= n; k ++)
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
}
int main(){
    int n,m;
    while(~scanf("%d%d",&n,&m),n+m){
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                if(i == j)
                    g[i][j] = 0;
                else
                    g[i][j] = INF;
        for(int i = 1; i <= m; i++){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            g[u][v] = g[v][u] = w;
        }
        floyd(n);

        int ans = -1;
        printf("%d\n",g[1][n]);
    }
}
```

### poj1502

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
#define INF 100000000  
const int maxn = 105;
int g[maxn][maxn];
int min(int a,int b){
    return a<b?a:b;
}

void floyd(int n ){
    for(int k = 1; k <= n; k ++)
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
}
int main(){
    int n;
    while(~scanf("%d",&n)){
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                g[i][j] = INF;
        for(int i = 2; i <= n; i++)
            for(int j = 1; j < i; j++){
                int x;
                if(scanf("%d",&x))  g[i][j] = g[j][i] = x;
                else scanf("x"); //input handle
            }
        floyd(n);

        int ans = -1;
        for(int i = 1; i <= n; i++){
            if(g[1][i] > ans)
                ans = g[1][i];
        }
        printf("%d\n",ans);
    }
}
```