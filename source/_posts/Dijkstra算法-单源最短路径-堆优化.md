title: Dijkstra算法 单源最短路径 堆优化
tags: [图论,最短路径,dijkstra]
categories: 算法分析
date: 2015-09-05 10:46:00
---

## 概念

Dijkstra是用来解决只含有非负边权的单源最短路径问题。给定一个起始点s，算法可以求出s到其他所有点的最短路径距离。算法结束后会生成一棵最短路径树。
<!-- more -->
## 算法

*   输入：图G（邻接矩阵或者邻接表）
*   输出：s到每一个点的最短路径
*   具体流程：

    1.  初始化距离数组为INF
    2.  设置所有点都未访问过
    3.  找到所有未访问过的点中最小的一个，标记为访问过
    4.  对3中找到的点的所有相邻点进行松弛操作
    5.  重复3和4直到所有点都被访问过
*   复杂度：朴素方法`O(|V|^2+|E|)`，最大堆维护距离`O(|E|+|V|log|V|)`
*   算法动图

    ![dijkstra](http://pic002.cnblogs.com/images/2012/426620/2012073019540660.gif)

## 优化

朴素的dijkstra相对于spfa没有任何优势，复杂度达到了`O(n^2)`的级别，但是只要加入了堆优化，复杂度可以降到`O(nlog(n))`,则体现出它的优势了。最小堆查询最小值的复杂度为`O(logn)

### 最小堆模板

```cpp

```
struct HeapElement {  
    // key存储顶点序号，value存储到该顶点的最短距离   
    int key, value;  
};  
struct MinHeap {  
    HeapElement heap[MAXN];  
    int position[MAXN];  
    int size; // 顶点数  
    void init() {   
        heap[size=0].value = -INF;   
        memset(position, 0, sizeof(position));  
    }  
    void insert(int key, int value) {  
        heap[++size].key = key;  
        heap[size].value = value;  
        position[key] = size;  
        siftUp(size);  
    }  
    void decrease(int index) {  
        int x = heap[index].value;  
        int y = heap[size].value;  
        -- size;  
        if (index == size+1)  
            return;  
          
        swap(heap[index], heap[size+1]);  
        if (y >= x) {  
            siftDown(index);      
        } else {  
            siftUp(index);    
        }  
    }  
    int delmin() {  
        int x = heap[1].key;  
        decrease(1);  
        return x;  
    }  
    void siftUp(int index) {  
        while (index > 1) {  
            if (heap[index].value < heap[index/2].value) {  
                swap(heap[index],heap[index/2]);      
            } else {  
                break;  
            }  
            index /= 2;  
        }  
    }  
    void siftDown(int index) {  
        while (index*2 <= size) {  
            index *= 2;  
            if (index < size && heap[index].value > heap[index+1].value) {  
                ++ index;  
            }  
            if (heap[index].value < heap[index/2].value) {  
                swap(heap[index],heap[index/2]);  
            } else {  
                break;  
            }  
        }  
    }  
    void makeHeap() {  
        for (int i = size/2; i > 0; -- i)   
            siftDown(i);  
    }  
    void swap(HeapElement &a, HeapElement &b) {  
        HeapElement temp = a;  
        a = b;  
        b = temp;  
        int tmp = position[a.key];  
        position[a.key] = position[b.key];  
        position[b.key] = tmp;  
    }  
}H;  

## 应用

### hdu 2544

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
#define INF 0x3f3f3f3f  
const int maxn = 1010;
typedef struct edge{
    int i,w;
};
vector<edge> g[maxn];
int dis[maxn];
int vis[maxn];
int min(int a,int b){  
    return a<b?a:b;  
} 
void dijkstra(int n){
    for(int i = 1; i <= n ; i++)
        dis[i] = INF;
    dis[1] = 0;
    memset(vis, 0, sizeof(vis));
    for(int i = 1; i <= n; i++){
        int mark = -1;
        int mindis = INF;
        for(int j = 1; j <= n; j++){
            if(!vis[j] && dis[j] < mindis){
                mindis = dis[j];
                mark = j;
            }
        }//find vertex of minimal distance
        vis[mark] = 1;
        for(int j = 0; j < g[mark].size(); j++){
            int v = g[mark][j].i;
            int w = g[mark][j].w;
            if(!vis[v]){
                dis[v] = min(dis[v],dis[mark]+w);
            }       
        }//relax the adjacent edges of minimal vertex
    }
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m),m+n){
        memset(g, 0, sizeof(g));
        for(int i = 1; i <= m; i++){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            edge e;
            e.i = v;
            e.w = w;
            g[u].push_back(e);
            e.i = u;
            g[v].push_back(e);
        }
        dijkstra(n);
        printf("%d\n",dis[n]);
    }   
} 
```
### hdu 2544 堆优化版本

```cpp
#include <iostream>  
#define INF 0x7FFFFFFF  
using namespace std;  
  
const int SIZE = 105;  
int dist[SIZE];  
int G[SIZE][SIZE];  
bool vis[SIZE];  
struct HeapElement {  
    int key, value;      
};  
void swap(HeapElement &ha, HeapElement &hb) {  
    int key = ha.key;  
    int value = ha.value;  
    ha.key = hb.key;  
    ha.value = hb.value;  
    hb.key = key;  
    hb.value = value;  
};  
// 使用邻接表储存图，线性表储存堆   
struct MinHeap {  
    HeapElement heap[SIZE];  
    int n;    // 顶点数      
          
    void makeheap() {  
        for (int i = n/2; i > 0; -- i)  
            siftDown(i);  
    };  
    void siftUp(int index) {  
        int k = index;  
        while (k > 1) {  
            if (heap[k].value < heap[k/2].value) {  
                swap(heap[k],heap[k/2]);      
            } else {  
                break;      
            }  
            k /= 2;      
        }  
    };  
    void siftDown(int index) {  
        int k = index;  
        while (k*2 <= n) {  
            k *= 2;  
            if (k < n && heap[k].value > heap[k+1].value) {  
                ++ k;      
            }  
            if (heap[k].value < heap[k/2].value) {  
                swap(heap[k],heap[k/2]);  
            } else {  
                break;      
            }      
        }  
    };  
    void insert(HeapElement element) {  
        heap[++n] = element;  
        siftUp(n);      
    };  
    void decrease(int index) {  
        int x = heap[index].value;  
        int y = heap[n].value;  
        n -= 1;  
          
        // 若删除节点位于最末位置，则删除成功，无需其他操作。   
        if (index == n+1)   
            return;  
          
        heap[index] = heap[n+1];  
        if (y >= x) {  
            siftDown(index);      
        } else {  
            siftUp(index);      
        }  
    };  
    int decreaseMin() {  
        int x = heap[1].key;  
        decrease(1);  
        return x;  
    };  
}H;  
  
void dijkstra(int src, int n) {  
    int i, j, w;  
    bool flag;  
      
    for (i = 1; i <= n; ++ i) {  
        if (G[i][src] != INF) {  
            dist[i] = G[src][i];  
            HeapElement h = {i, dist[i]};  
            H.insert(h);      
        } else {  
            dist[i] = INF;      
        }      
    }  
      
    memset(vis, false, sizeof(vis));  
    vis[src] = true;  
    dist[src] = 0;  
      
  
      
    for (i = 1; i < n; ++ i) {  
  
        int node = H.decreaseMin();  //get the vertex of minimal distance
        vis[node] = true;  
  
        for (w = 1; w <= n; ++ w) {  
            flag = false;  
            if (!vis[w] && G[node][w] != INF) {  
                if (dist[node] < dist[w] - G[node][w]) {  
                    dist[w] = dist[node] + G[node][w];   //relax all adjacent of this vertex
                          
                }   
                for (j = 1; j <= H.n; ++ j) {       //update minimal heap
                        if (H.heap[j].key == w) {  
                            H.heap[j].value = dist[w];  
                            flag = true;  
                            break;      
                        }      
                    }      
                  
                if (!flag) {  
                    HeapElement h = {w, dist[w]};  
                    H.insert(h);  
                } else {  
                    H.siftUp(j);  
                }  
            }  
        }      
    }  
};  
  
void init(int n) {  
    for (int i = 1; i <= n; ++ i)  
        for (int j = 1; j <= n; ++ j)  
            G[i][j] = INF;  
    H.n = 0;  
};  
  
int main()  
{  
    int N, M, a, b, c;  
      
      
    while (scanf("%d%d",&N,&M)!=EOF, N&&M) {  
        init(N);  
          
        for (int i = 0; i < M; ++ i) {  
            scanf("%d%d%d",&a,&b,&c);  
            if (G[a][b] > c) {  
                G[a][b] = c;  
                G[b][a] = c;  
            }  
        }  
          
        dijkstra(1, N);  
          
        printf("%d\n",dist[N]);      
    }  
}  
```
 

### poj 1502
```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
#define INF 0x3f3f3f3f  
const int maxn = 1010;
int dis[maxn],g[maxn][maxn];
int vis[maxn];
int min(int a,int b){
    return a<b?a:b;
}
void dijkstra(int n){
    for(int i = 1; i <= n; i++)
        dis[i] = INF;
    dis[1] = 0;
    memset(vis, 0 , sizeof(vis));
    for(int i = 1; i <= n; i++){
        int mark = -1;
        int mindis = INF;
        for(int j = 1; j <= n; j++){
            if(!vis[j] && dis[j] < mindis){
                mindis = dis[j];
                mark = j;
            }
        }
        vis[mark] = 1;
        for(int j = 1; j <= n; j++){
            if(!vis[j]){
                dis[j] = min(dis[j], g[mark][j] + dis[mark]);
            }
        }
    }
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
        dijkstra(n);
        int ans = -1;
        for(int i = 1; i <= n; i++){
            if(dis[i] > ans)
                ans = dis[i];
        }
        printf("%d\n",ans);
    }
}
```
 