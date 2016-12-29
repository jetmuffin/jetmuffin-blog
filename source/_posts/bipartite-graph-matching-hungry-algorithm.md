title: 二分图的最大匹配 匈牙利算法
date: 2015-11-01 19:19:16
tags: 二分图,图论,匈牙利算法
categories: 算法分析
---
# 二分图的最大匹配 匈牙利算法

## 概念

**匹配:**

> 给定一个无向图G=(V, E),一个匹配是边的一个子集M⊇E，使得对于所有节点v∈V，子集M中最多有一条边与节点v相连。
—— 《算法导论》
<!-- more -->


在图论中，一个「匹配」（matching）是一个边的集合，其中任意两条边都没有公共顶点。例如，图 3、图 4 中红色的边就是图 2 的匹配。
![img](http://i11.tietuku.com/e19acdb1257218aa.png)


我们定义**匹配点**、**匹配边**、**未匹配点**、**非匹配边**，它们的含义非常显然。例如图 3 中 1、4、5、7 为匹配点，其他顶点为未匹配点；1-5、4-7为匹配边，其他边为非匹配边。

**最大匹配：**一个图所有匹配中，所含匹配边数最多的匹配，称为这个图的最大匹配。图 4 是一个最大匹配，它包含 4 条匹配边。

**完美匹配：**如果一个图的某个匹配中，所有的顶点都是匹配点，那么它就是一个完美匹配。图 4 是一个完美匹配。显然，完美匹配一定是最大匹配（完美匹配的任何一个点都已经匹配，添加一条新的匹配边一定会与已有的匹配边冲突）。但并非每个图都存在完美匹配。

## 定理

**最大匹配数**：最大匹配的匹配边的数目
**最小点覆盖数**：选取最少的点，使任意一条边至少有一个端点被选择
**最大独立数**：选取最多的点，使任意所选两点均不相连

* 定理1：最大匹配数 = 最小点覆盖数（这是 Konig 定理）
* 定理2：最大匹配数 = 最大独立数
* 定理3：最小路径覆盖数 = 顶点数 - 最大匹配数

## 匈牙利算法

![img2](http://img.renfei.org/2013/08/2.png)

**思路：**匈牙利算法的主要思路是希望每一个左边的点都尽量在右边找到一个点和他匹配，若找不到匹配点，则想方设法给它**腾出**点来供他匹配。

**流程：**我们依次枚举左边的点x(`如图2中的2`)的所有出边指向点y（`如图2中的5`），若y之前没有被匹配，则(x,y)成为一对匹配，我们将匹配数加一；若y已经被匹配（`假设图2中的5已经被1匹配）`），那么我们需要将匹配y的x'重新找一个匹配（`即给1再重新找一个匹配`），若找到x'的新匹配（` 找到了7`)，那么(x,y)还是能够成为一对匹配。

**例子：**
原图为：
![img3](http://img.renfei.org/2013/08/1.png)
顶点分为两类：
![img4](http://img.renfei.org/2013/08/2.png)
首先对顶点1进行匹配，找到匹配点5
![img5](http://i11.tietuku.com/4f5f157609901e7e.png)
然后对顶点2进行匹配，发现没有任何顶点可以匹配，于是对顶点5的原匹配点再重新找一个匹配，找到了匹配7
![img6](http://i11.tietuku.com/36347ef2b669664e.png)
接下来对顶点3配对，找到匹配6
![img7](http://i11.tietuku.com/ab0b9f0bceb265c1.png)
最后将4和8进行配对
![img8](http://i11.tietuku.com/cb3bf6c5aeb5a275.png)
得到原图的最大匹配为4

**伪代码：**
```
function Find(u):
    对于u的每一个相邻的，且没有被标记过的节点v:
        标记v;
        如果v未饱和（未得到匹配）或者Find(v的当前匹配点):
            将v的匹配点修改为u
            返回True
        返回False
```

```
function Main():
    将所有节点初始化为未饱和;
    匹配数=0;
    对于图中的每一个顶点u:
        清空所有标记;
        如果Find(u)，匹配数+=1;
    返回匹配数
```

**复杂度：**即对于每个顶点，将 每条边遍历一遍，复杂度`O(VE)`

**实现代码：**
```cpp
bool dfs(int u)//从左边开始找增广路径
{
    int v;
    for(v=0;v<N;v++)//这个顶点编号从0开始，若要从1开始需要修改
      if(g[u][v]&&!used[v])
      {
          used[v]=true;
          if(link[v]==-1||dfs(link[v]))
          {//找增广路，反向
              link[v]=u;
              return true;
          }
      }
    return false;//这个不要忘了，经常忘记这句
}
int main()
{
    int res=0;
    int u;
    memset(link,-1,sizeof(link));
    for(u=0;u<N;u++)
    {
        memset(used,0,sizeof(used));
        if(dfs(u)) res++;
    }
    return res;
}
```

## Hopcroft Karp算法

Hopcroft Karp算法是对匈牙利算法的实现，每次不止增广一个非饱和顶点。复杂度`O(sqrt(V)E)

**实现代码：**
```cpp
bool searchP()  
{  
    dis=INF;  
    int i,v,u;  
    queue<int> Q;  
  
    memset(dx,-1,sizeof(dx));  
    memset(dy,-1,sizeof(dy));  
    for(i=0;i<Nx;i++)  
    {  
        if(Mx[i]==-1)  
        {  
            Q.push(i);  
            dx[i]=0;  
        }  
    }  
    while(!Q.empty())  
    {  
        u=Q.front();  
        Q.pop();  
        if(dx[u]>dis) break;  
        for(v=0;v<Ny;v++)  
        {  
            if(g[u][v]&&dy[v]==-1)  
            {  
                dy[v]=dx[u]+1;  
                if(My[v]==-1) dis=dy[v];  
                else  
                {  
                    dx[My[v]]=dy[v]+1;  
                    Q.push(My[v]);  
                }  
            }  
        }  
    }  
    return dis!=INF;  
}  
  
bool DFS(int u)  
{  
    int v;  
    for(v=0;v<Ny;v++)  
    {  
        if(g[u][v]&&!used[v]&&dy[v]==dx[u]+1)  
        {  
            used[v]=true;  
            if(My[v]!=-1&&dy[v]==dis) continue;  
            if(My[v]==-1||DFS(My[v]))  
            {  
                My[v]=u;  
                Mx[u]=v;  
                return true;  
            }  
        }  
    }  
    return false;  
}  
  
int Hungary()  
{  
    int u;  
    int ret=0;  
    memset(Mx,-1,sizeof(Mx));  
    memset(My,-1,sizeof(My));  
    while(searchP())  
    {  
        memset(used,false,sizeof(used));  
        for(u=0;u<Nx;u++)  
           if(Mx[u]==-1&&DFS(u))  ret++;  
    }  
    return ret;  
}  
```