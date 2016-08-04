title: 'hdu 5105 Math Problem BestCoder#18 1002 求最大值'
tags: []
date: 2014-11-19 22:07:59
---

<span style="white-space:pre"></span>f(x) = ax^3+bx^2+cx+d，最大&#20540;只会出现在几个地方：左端点，右端点，极&#20540;点。

<span style="white-space:pre"></span>求导后是个二次函数，可以求根，然后判断根是否在(L,R)范围内，若在范围内，直接求max(f(x),f(L),f(R))就是最大&#20540;了。

<span style="white-space:pre"></span>PS：要考虑a=0，b=0的情况，WA了一次ORZ

<!--more-->

<span style="white-space:pre"></span>`#include <iostream>

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

//#define WE freopen("out.txt","w",stdout) 

const double INF = 99999;

double a,b,c,d,L,R;

double max(double a,double b)

{

	return a>b?a:b;

}

double fun(double x)

{

	return fabs(a*x*x*x+b*x*x+c*x+d);

}

int main()

{

	while(~scanf("%lf %lf %lf %lf %lf %lf",&a,&b,&c,&d,&L,&R))

	{

		double ans;

		double left = fun(L);

		double right = fun(R);

		double sans = max(left,right);

		if(a==0)

		{

			if(b == 0){

				ans = sans;

			}else{

				double x = -c/(2*b);

				if(x<=R&&x>=L)

					ans = fun(x);

				else 

					ans = 0;

				ans = max(ans,sans);				

			}

		}

		else{

			double deta = 4*b*b - 12*a*c;

			if(deta < 0){

				ans = 0;

				ans = max(ans,sans);

			}

			else{

				double x1 = (sqrt(deta)-2*b)/(6*a);

				double x2 = (-sqrt(deta)-2*b)/(6*a);

				double k1,k2;

				if(x1<=R&&x1>=L)

					k1 = fun(x1);

				else

					k1 = 0;

				if(x2<=R&&x2>=L)

					k2 = fun(x2);

				else

					k2 = 0;	

				ans = max(k1,k2);

				ans = max(ans,sans);

			}					

		}

		printf("%.2lf\n",ans);

	}

}`