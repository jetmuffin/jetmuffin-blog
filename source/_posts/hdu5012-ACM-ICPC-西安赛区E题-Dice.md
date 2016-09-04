title: hdu5012 ACM-ICPC 西安赛区E题 Dice
tags: [bfs,ACM网络赛]
categories: 算法题解
date: 2014-09-14 22:04:22
---

题目的意思是给定骰子的一个开始状态（六个面的数字），再给定另一个结束状态，问从初始到结束状态最少要走几步，只能前后左右翻滚。

一开始做题想简单了，以为模拟所有方案就可以了，后来发现模拟的越多越乱，结果没A出来。

比赛结束后尝试BFS+Hash(记忆化搜索)，先确定四种操作各改变的数组元素，然后搜索，便可以A了。

代码不够优美，毕竟菜鸟：

<!--more-->

代码：
```cpp
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

//#define mod 1000000007

#define eps 1e-8

#define MP make_pair

#define RE freopen("in.txt","r",stdin)

#define WE freopen("out.txt","w",stdout) 

const int maxn=10000;

string a,b;

char tmp[100],tmp2[100];

int hash[maxn];

string L(string c)

{

	char t = c[2];

	c[2] = c[0];

	c[0] = c[3];

	c[3] = c[1];

	c[1] = t;

	return c;

}

string R(string c)

{

	char t = c[1];

	c[1] = c[3];

	c[3] = c[0];

	c[0] = c[2];

	c[2] = t;

	return c;

}

string F(string c)

{

	char t = c[0];

	c[0] = c[5];

	c[5] = c[1];

	c[1] = c[4];

	c[4] = t;

	return c;

} 

string B(string c)

{

	char t = c[4];

	c[4] = c[1];

	c[1] = c[5];

	c[5] = c[0];

	c[0] = t;

	return c;

}

struct node{

	string s;

	int step;

	void print()

	{

		cout<<s<<endl;

	}

	unsigned int APHash()

	{

	    unsigned int key = 0;

	    int i;

	    for (i=0; i<s.length(); i++)

	    {

	        if ((i & 1) == 0)

	            key ^= ((key << 7) ^ (s[i]) ^ (key >> 3));

	        else

	            key ^= (~((key << 11) ^ (s[i]) ^ (key >> 5)));

	    }

	    return (key & 0x7FFFFFFF)%maxn;

	}

};

void rotate(int i,node &p,node q)

{

	if(i==1) p.s = L(q.s);

	if(i==2) p.s = R(q.s);

	if(i==3) p.s = F(q.s);

	if(i==4) p.s = B(q.s);

}

node p;

void print()

{

	cout<<a<<endl;

	cout<<b<<endl;

	cout<<endl;

}

int bfs() {  

	memset(hash,0,sizeof(hash));

    int j,key;  

    queue<node> q;

    q.push(p); 

    while(!q.empty()) {  

        node p = q.front();

        if(p.s == b)

		{

			return p.step;

		}

		node l,r,f,b;

		int ll,rr,ff,bb;

		rotate(1,l,p);ll=l.APHash();

		rotate(2,r,p);rr=r.APHash();

		rotate(3,f,p);ff=f.APHash();

		rotate(4,b,p);bb=b.APHash();

		if(!hash[ll]){

			l.step=p.step+1;

			hash[ll] = 1;

			q.push(l);

		}   

		if(!hash[rr]){

			r.step=p.step+1;

			hash[rr] = 1;

			q.push(r);

		}

		if(!hash[ff]){

			f.step=p.step+1;

			hash[ff] = 1;

			q.push(f);

		}

		if(!hash[bb]){

			b.step=p.step+1;

			hash[bb] = 1;

			q.push(b);

		}	

//		cout<<"bfs: "<<p.s<<" hashKey:"<<p.APHash()<<" depth:"<<p.step<<endl;

		q.pop();

    }

	return -1;

}

int main()

{

	//RE;

	while(gets(tmp)&&gets(tmp2))

	{

		a="";

		b="";

		for(int i=0;i<strlen(tmp);i++)

			if(tmp[i]!=' ') a+=	tmp[i];

		for(int i=0;i<strlen(tmp2);i++)

			if(tmp2[i]!=' ') b+=	tmp2[i];		

		p.s=a;

		p.step=0;

		cout<<bfs()<<endl;

	}

}
```