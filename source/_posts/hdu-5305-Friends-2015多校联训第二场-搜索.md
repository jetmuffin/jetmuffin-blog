title: hdu 5305 Friends 2015多校联训第二场 搜索
tags: [dfs,搜索]
categories: 算法
date: 2015-07-26 16:55:00
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=5305](http://acm.hdu.edu.cn/showproblem.php?pid=5305)

## 题意

对题意的理解可以抽象成，当前已知一个`图`，图最多的点数为`8`,然后将图中的部分边进行`染色`，然后使得每个点对应的边有一半是黑边，一半是白边。

## 分析

一开始接触题的想法是，既然数据范围小，可以直接进行搜索，然而一开始的策略是对点进行搜索，每个点枚举一半的边，进行染色，然后再搜下一个点进行染色，方法非常复杂，后来就没用继续下去了。

然而及时看了下`staus`发现大家过得时间非常短，一般都是`15ms`或`30ms`就通过了，然后一度以为是结论题，只要统计边和点的信息就可以解出题了，结果不是。

看了题解后发现做法应该是按边取搜索。可以先进行一次粗略的`剪枝`，统计每个点的度数，若出现奇数直接输出0；

然后将每个点的`白边数`和`黑边数`分别置为度数的一半，然后枚举保存的边，分别按染白和染黑进行搜索，知道搜索出结果。

## 代码

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
      #define MP make_pair

      #define mod 1000000007
      #define eps 1e-8
      #define pi acos(-1.0)
      #define inf 0x3fffffff
      int n,m;
      struct edge{
          int u,v;
      }e[100];
      int deg[10];
      int c1[10];
      int c2[10];
      int ans = 0;
      void dfs(int cur){
      //    cout<<"cur: "<<cur<<endl;
      //    for(int i = 1; i <= n; i ++){
      //        cout<<c1[i]<<" ";
      //    }
      //    cout<<endl;
      //    for(int i = 1; i <= n; i ++){
      //        cout<<c2[i]<<" ";
      //    }
      //    cout<<endl;    
          if(cur == m+1){
              ans++;
              return;
          }
          int v = e[cur].v;
          int u = e[cur].u;
          if(c1[u] && c1[v]){
              c1[u] --;
              c1[v] --;
              dfs(cur+1);
              c1[u] ++;
              c1[v] ++;
          }
          if(c2[u] && c2[v]){
              c2[u] --;
              c2[v] --;
              dfs(cur+1);
              c2[u] ++;
              c2[v] ++;
          }
          return;
      }
      int main()
      {
          #ifndef ONLINE_JUDGE
              freopen("in.txt","r",stdin);
              //freopen("out.txt","w",stdout);
          #endif

          int t;
          scanf("%d",&t);
          while(t--){
              scanf("%d%d",&n,&m);
              ans = 0;
              memset(deg,0,sizeof(deg));
              memset(c1,0,sizeof(c1));
              memset(c2,0,sizeof(c2));
              for(int i = 1; i <=m; i++){
                  scanf("%d%d",&e[i].u,&e[i].v);
                  deg[e[i].v]++;
                  deg[e[i].u]++;
              }    
              int flag = 1;
              for(int i = 1; i <= n; i++){
                  if(deg[i] & 1){
                      flag = 0;
                      break;
                  }
                  c1[i] = deg[i]/2;
                  c2[i] = deg[i]/2;
              }
              if(!flag){
                  puts("0");
                  continue;
              }
              dfs(1);
              printf("%d\n",ans);
          }
      } 
    