title: hdu 2080 夹角有多大II
tags: [hdu,几何]
categories: 算法
date: 2014-10-15 04:38:35
---

<span style="white-space:pre"></span>这是一题水题，至于为什么放在这儿。。因为要借鉴一下做法。。

<span style="white-space:pre"></span>地区赛的时候一道题之中要用到该题的结论，当时的做法是算(x轴-该点与原点连线)，再求其arctan算出角度，取两个角度求差。

<span style="white-space:pre"></span>这种做法的弊端就是，如果角度大于90度，大于180度，大于270度，得做相应处理，同时如果某点在坐标轴上也得处理，非常的繁琐。

<span style="white-space:pre"></span>今天做到这道水题才发现问题所在，原来直接用余弦定理一步搞定，cosA = (a^2 + b^2 - c^2)/(2*a*b)，同时也不用处理180度的问题。以后就不会犯这种错了。

<span style="white-space:pre"></span>需要提的一点是，x,y都是double，没发现这点用int，WA了数次。。不够细心

<!--more-->

<span style="white-space:pre"></span>代码：

`#include <iostream>

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

#define REP(i,a,b) for(int i = a; i < b; ++i)

#define RREP(i,a,b) for(int i = b; i > a ; --i)

#define RE freopen("in.txt","r",stdin)

#define WE freopen("out.txt","w",stdout) 

#define PI 3.141592654

int main()

{

	int T;

	cin>>T;

	while(T--)

	{

		double x1,x2,y1,y2;

		cin>>x1>>y1>>x2>>y2;

		double a=(double)sqrt((double)(x1*x1+y1*y1));

		double b=(double)sqrt((double)(x2*x2+y2*y2));

		double c=(double)sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

		double arc=acos((a*a+b*b-c*c)/(2*a*b));

		arc=arc*180/PI;

		printf("%.2f\n",arc);

	}

}`