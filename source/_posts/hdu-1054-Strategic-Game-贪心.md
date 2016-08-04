title: hdu 1054 Strategic Game 贪心
tags: [队列,贪心]
categories: 算法
date: 2015-07-27 16:05:00
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=1054](http://acm.hdu.edu.cn/showproblem.php?pid=1054)

## 题意

给定一个二分图（给图的方式太蛋疼，输入输出好麻烦），在任意节点放置哨兵使得可以覆盖整个图。图的节点数`n <= 1500`

## 分析

这题看似一道很裸的二分图最小点覆盖（或者最大匹配），然而我并不会这些内容，所以一直在想其他策略。

一开始有一种贪心思路是，将点以度数进行排序，因为取度数较大的点上放一个哨兵的收益最大，因此需要动态维护一个堆，使得这个堆堆顶始终是度数最大的点，然后从堆顶取点，并将点对应的相邻点度数一次减1，然后将度数为0的点直接踢出堆外以减小复杂度。而这个堆可以是二叉堆，修改、删除、插入、取堆顶元素的操作复杂度均为`O(logn)`，然而我这块儿也学得不好，直接敲模板又比较麻烦。

所以从思路二继续尝试其他思路，发现从相反反向考虑，度数较大的点放哨兵收益最大，而度数最小的点（二分图无孤立点）即度数为1的点放哨兵度数收益最小，因此从反方向考虑这个问题的话，就是尽量不要再度数为1的点上放哨兵，而是用它的邻点取代它。因此我们维护一个队列，放置所有度数为1的点，每次取出队首`u`，并取出第一个邻点`v`，将邻点`v`的所有邻点度数更新，若出现度数为1的点则再加入队列。

## 代码

    #include <iostream>
    #include <cstdio>
    #include <cstring>
    #include <vector>
    #include <algorithm>
    #include <queue>
    using namespace std;
    const int maxn = 1600;
    int deg[maxn];
    int vis[maxn];
    vector<int> edge[maxn];
    int main(){
        int n;
    //  freopen("in.txt","r",stdin);
        while(~scanf("%d",&n)){
            memset(deg,0,sizeof(deg));
            memset(vis,0,sizeof(vis));
            memset(edge,0,sizeof(edge));

            for(int i = 0 ; i < n ; i++){
                int m,u;
                scanf("%d:(%d)",&u,&m);
                if(m > 0){
                    for(int j = 0 ; j < m; j++){
                        int v;
                        scanf("%d",&v);
                        edge[u].push_back(v);
                        edge[v].push_back(u);
                        deg[u]++;
                        deg[v]++;
                    }
                }
            }
    //      for(int i = 0 ; i < n; i++){
    //          cout<<deg[i]<<" ";
    //      }
    //      cout<<endl;
            if(n == 1){
                puts("1");
                continue;
            }           
            queue <int> q;
            for(int i = 0 ; i < n; i++){
                if(deg[i] == 1)
                    q.push(i);
            }
            int ans = 0;
            while(!q.empty()){
                int u = q.front();
                q.pop();
                if(vis[u])
                    continue;
                for(int i = 0; i < edge[u].size(); i++){
                    int v = edge[u][i];
                    if(!vis[v]){
                        vis[v] = 1;
                        ans++;
                        //选1个放烧饼 
                        for(int j = 0; j < edge[v].size(); j++){
                            int w = edge[v][j];
                            deg[w] --;
                            if(deg[w] == 1 && !vis[w]){
                                q.push(w);
                            }
                        }
                        break; 
                    }
                }
            }

            cout<<ans<<endl;
        }

    }
    