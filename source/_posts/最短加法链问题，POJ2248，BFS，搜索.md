title: 最短加法链问题，POJ2248，BFS，搜索
tags: [poj,bfs]
categories: 算法
date: 2014-10-30 07:17:18
---

<pre name="code" class="cpp">
</pre>

该题也是算法导论的结课作业，看了一下貌&#20284;在POJ上有原题。

<span style="white-space:pre"></span>主要思路就是搜索，至于DFS还是BFS都可以，DFS需要加比较好的剪枝函数，不然会TLE。

<span style="white-space:pre"></span>

宽搜<span style="font-family:Times New Roman,serif"><span lang="en-US">BFS</span></span>，因为要找到的是最短的加法链，宽搜是最快速的方法。算法的设计上，因为需要保存路径，所以用结构体<span style="font-family:Times New Roman,serif"><span lang="en-US">node</span></span>（<span style="font-family:Times New Roman,serif"><span lang="en-US">intid,int

 val,int pre)</span></span>保存每个节点，根据<span style="font-family:Times New Roman,serif"><span lang="en-US">pre</span></span>和<span style="font-family:Times New Roman,serif"><span lang="en-US">id</span></span>的映射关系使得路径成为链式，再宽搜路径即可

<!--more-->

<span style="white-space:pre"></span>核心代码代码：

<span style="white-space:pre"></span>

`void bfs()

{

    q.push(node(0,1,-1));

    path[cnt++] = node(0,1,-1);

    node now,next;

    while(!q.empty())

    {

        now = q.front();

        q.pop();

        //now.print();

        for(int i = now.id; i!=-1; i = path[i].pre)

        {

            int tmp = now.val + path[i].val;

            if(tmp == n)

            {

               cout<<countPath(now.id)<<endl;

                cout<<n<<" ";

                output(now.id);

                cout<<endl;

                return;

            }

            if(tmp < n)

            {

                next = node(cnt, tmp, now.id);

                path[cnt++] = next;

                q.push(next);

            }

        }

    }

}`
<span style="white-space:pre"></span>

<p class="cjk" align="justify" style="text-indent:0.76cm; line-height:150%; page-break-inside:auto; widows:0; orphans:0; page-break-before:auto; page-break-after:auto">