title: 最短加法链问题，POJ2248，BFS，搜索
tags: [加法链,bfs]
categories: 算法题解
date: 2014-10-30 07:17:18
---


该题也是算法导论的结课作业，看了一下貌似在POJ上有原题。


宽搜`BFS`，因为要找到的是最短的加法链，宽搜是最快速的方法。算法的设计上，因为需要保存路径，所以用结构体`node（intid,int val, int pre)，保存每个节点，根据`pre`和`id`的映射关系使得路径成为链式，再宽搜路径即可

<!--more-->

核心代码代码：

```cpp

void bfs() {
    q.push(node(0,1,-1));
    path[cnt++] = node(0,1,-1);
    node now,next;
    while(!q.empty()) {
        now = q.front();
        q.pop();
        //now.print();
        for(int i = now.id; i!=-1; i = path[i].pre) {
            int tmp = now.val + path[i].val;
            if(tmp == n) {
                cout<<countPath(now.id)<<endl;
                cout<<n<<" ";
                output(now.id);
                cout<<endl;
                return;
            }

            if(tmp < n) {
                next = node(cnt, tmp, now.id);
                path[cnt++] = next;
                q.push(next);
            }
        }
    }
}

```