title: 'Codeforces #279 A Team Olympiad 瞎搞'
tags: [CodeForces,水题]
categories:  算法
date: 2014-11-25 00:30:27
---

<span style="white-space:pre"></span>难得一场CF在下午5点打，果断参加，涨了100多分～

<span style="white-space:pre"></span>A题比较水，说的是n个学生，分别擅长体育，数学， 编程，要求把他们分成几组，每组里必须有会编程，会体育，会数学的各一人，求最多分多少组。

<span style="white-space:pre"></span>其实就是将三类学生分类，找到人数最少的一类，瞎搞搞就过了。

<!--more-->

<span style="white-space:pre"></span>代码：

<span style="white-space:pre"></span>`#include <iostream>

#include <algorithm>

using namespace std;

const int maxn=5001;

int a[maxn];

int b[maxn];

int c[maxn];

int n;

int main()

{

    cin>>n;

    int ax = 0, bx = 0, cx = 0;

    for(int i = 0;i<n;i++)

    {

        int tmp;

        cin>>tmp;

        if(tmp == 1) {

            a[ax++] = i;

        }

        if(tmp == 2){

            b[bx++] = i;

        }

        if(tmp == 3){

            c[cx++] = i;

        }

    }

    if(ax==0||bx==0||cx==0){

        cout<<0<<endl;

        return 0;

    }

    int ans = 0;

    ans = min(ax,bx);

    ans = min(ans,cx);

    cout<<ans<<endl;

    for(int i = 0; i < ans; i++)

    {

        cout<<a[i]+1<<" "<<b[i]+1<<" "<<c[i]+1;

        cout<<endl;

    }

}`