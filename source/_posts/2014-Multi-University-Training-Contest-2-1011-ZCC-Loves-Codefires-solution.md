title: 2014 Multi-University Training Contest 2 1011 ZCC Loves Codefires 解题报告
tags: [dp,多校联训]
categories: 算法题解
date: 2014-07-24 11:06:19
---

暑期联训第二场。

高中被镇海虐了3年，大学继续被镇海虐，刚入ACM这条路不久，基础不够扎实，镇海还出这么变态的题目= =，就AC了最后一题。。

题目如上，总结就是类似分顺序取数，每个数大小不同，但是取出顺序会累加从而影响最后的结果，为了使最后结果最小，要找到最优解。

第一下看到这个题目，第一反应就是用动态规划去做，但是琢磨半天很难想出状态方程，因为它每个状态点无法比较状态对应的值大小（即使比较了也无意义）。

多次尝试之后，开始寻求其他方法，想了一种最朴素的方法，

<!--more-->

定义结构体
```
point{  

    int E;  

    int K;  

}
```

先用快速排序将按分值将point从大到小排序，然后依次从大到小进行冒泡，每次冒泡能保证前几次会使函数值减小，当再冒泡函数值增加时就停止该数的冒泡，换第二大的数进行冒泡。但是该算法每进行一次冒泡需要计算一次函数值，且n过大，最后导致TLE。

之后开始寻找变换的规律，

写出一组数据：

10 10 20 30

1     2   3   4

totEi	30 	50	60	70

Ki	4	3	2	1

交换3 和 4 后得到

totEi	20	50	60	70

Ki	3	4	2	1 

可以看出，3和4的交换，对于其他项是不会有影响的，对值的影响来源于30*4+50*3与20*3+50*4的大小变换。

用常规变量表示的话就是，point(i)>point(i+1) 的充要条件是  Ei+1*Ki+1+(Ei+Ei+1)*Ki<Ei*Ki+(Ei+1+Ei)*Ki+1

化简得 Ei*Ki+1<Ei+1*Ki 即Ei/Ki > Ei+1/Ki+1

所以只要该条件满足，交换point(i)与point(i+1）就能得到最优解。

因而在point结构体中加入变量double s  // s=E/K;

根据s对 point进行快速排序，排序得到的就是最优解，求和就是要出的结果了。

代码
```cpp
#include<algorithm>
using namespace std;
struct point{
	int E;
	int K;
	double s;
};

struct point p[100001]; 
int cmp(point p1,point p2) {
	return p1.s<p2.s;
}

int main() {
	freopen("in.txt","r",stdin);
	int i,j,N;
	int min;

	__int64 sum=0;
	__int64 totE=0;

	cin>>N;
	for(i=1;i<=N;i++)
		cin>>p[i].E;
	for(i=1;i<=N;i++)
		cin>>p[i].K;

	for(i=1;i<=N;i++)
		p[i].s = p[i].E/(double)p[i].K;
	sort(p+1,p+N+1,cmp);

	for(i=1;i<=N;i++) {
		totE+=p[i].E;
		sum+= totE*p[i].K;
	}

	cout<<sum<<endl;
}
```