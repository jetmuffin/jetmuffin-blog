title: 遗传算法求x^2+y^2的最大值，C++实现
tags: [遗传算法]
date: 2014-10-08 12:51:43
categories: 算法分析
---

最近和学院的教授开始学习数据挖掘和机器学习的课程，第一步就是学习简单的遗传算法。

在网上找了篇教程，于是将其实现。文章地址[非常好的理解遗传算法的例子](http://blog.csdn.net/b2b160/article/details/4680853/)。

内容是求求f(x1,x2)=x1^2+x2^2的最大值 ，x1∈{1,2,3,4,5,6,7} , x2∈{1,2,3,4,5,6,7} 。

选择样本数量为8，经过100代的迭代，效果还不错

<!--more-->

代码如下：

```cpp
/*

**		遗传算法求f(x1,x2)=x1^2+x2^2的最大值 

**		x1∈{1,2,3,4,5,6,7} , x2∈{1,2,3,4,5,6,7} 

**		@Jet-Muffin

*/ 

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
#define SAMPLE_NUM 8     //样本数量 
#define EVOLVE_NUM 300   //进化代数 
#define random(x) (rand()%x)   //随机函数 
#define VARIATE_RATE 10  //变异率 

void init();
void output();
void cross();
void cross_change(int a,int b);

//样本node 
struct node{
	int bin[6];
	int x1,x2;
	int f;
	double pct;
}group[SAMPLE_NUM];

//样本集合group 
node temp[SAMPLE_NUM];    //操作中间变量数组 
int choosen[SAMPLE_NUM];  //概率分布区域数组 

//初始化，随机生成样本 
void init() {
	for(int i = 0; i < SAMPLE_NUM; i++) {
		//随机生成bin数组 
		for(int j = 0; j < 6; j++) {
			int pct = random(100);  
			if(pct >= 50) 
				group[i].bin[j] = 1;
			else
				group[i].bin[j] = 0; 
		}
	}
}

//计算相应数值 

void calcu() {
	for(int i = 0; i < SAMPLE_NUM; i++) {
		//生成相应x1、x2
		int base = 1;
		group[i].x1 = group[i].x2 = 0;

		for(int j = 6 / 2 - 1; j >= 0; j--) {
			group[i].x1 += base * group[i].bin[j];	
			base *= 2;
		} 

		base = 1;
		for(int j = 6 - 1; j >= 6 / 2; j--) {
			group[i].x2 += base * group[i].bin[j];	
			base *= 2;
		} 

		//计算相应适值
		group[i].f = group[i].x1 * group[i].x1 + group[i].x2 * group[i].x2;
		
		//计算占总数百分比 
		int totf = 0;
		for(int i = 0; i < SAMPLE_NUM; i++)
			totf += group[i].f;
		for(int i = 0; i < SAMPLE_NUM; i++)
			group[i].pct = group[i].f / (double)totf; 
	}
}

//选择过程 
void choose() {
	cout<<endl;
	cout<<"Choosen:......."<<endl;
	memset(choosen,0,sizeof(choosen));

	//计算概率区域 
	choosen[0] = floor(group[0].pct * 100 + 0.5);
	for(int i = 1; i < SAMPLE_NUM - 1; i++) {
		choosen[i] = choosen[i-1] + floor(group[i].pct * 100 + 0.5);
	}

	choosen[SAMPLE_NUM - 1] = 100;
	cout<<"Posibility Area: ";
	for(int i = 0; i < SAMPLE_NUM; i++)
		cout<<choosen[i]<<" ";
	cout<<endl;

	//生成随机数以模拟选择 
	for(int i = 0; i < SAMPLE_NUM; i++) {
		int pct = random(100);
		int choosen_node;
		for(int j = 1; j < SAMPLE_NUM; j++) 
			if(pct < choosen[j]) {
				choosen_node = j;
				break;
			}
		cout<<"seed:"<<pct<<", ";
		cout<<"node "<<choosen_node + 1<<" was choosen"<<endl;
		temp[i] = group[choosen_node];	
	}

	//输出选择结果 
	cout<<"choosen result:"<<endl;
	for(int i = 0; i < SAMPLE_NUM; i++) {
		cout<<"temp["<<i + 1<<"]: ";
		for(int j = 0; j < 6; j++)
			cout<<temp[i].bin[j]<<" ";
		cout<<endl;	 
	}
}

//交叉运算 
void cross() {
	cout<<endl;
	cout<<"Crossing:...."<<endl;
	cout<<"Matching:..."<<endl;

	//用vector的random_shuffle实现随机乱序配对，从vector中取两个node就进行运算，实现随机交叉 
	vector<int> seeds;

	for (int i = 0; i < SAMPLE_NUM; i++)  
        seeds.push_back(i + 1);  

    random_shuffle(seeds.begin(), seeds.end());  
    int cnt = 0;
    int pair_fst,pair_sec;

    for(vector<int>::iterator it = seeds.begin(); it != seeds.end(); it++) {
    	cnt++;
    	if(cnt&1)
			pair_fst = *it;
		else
			pair_sec = *it;
		if(cnt == 2) {
			cout<<"Matching result: "<<pair_fst<<"-"<<pair_sec<<endl;
			cnt = 0;
			cross_change(pair_fst - 1,pair_sec - 1);
		}			
    }

    //输出交换结果 
	cout<<"cross result:"<<endl;
	for(int i = 0; i < SAMPLE_NUM; i++) {
		cout<<"temp["<<i + 1<<"]: ";
		for(int j = 0; j < 6; j++)
			cout<<temp[i].bin[j]<<" ";
		cout<<endl;	 
	}
    cout<<endl;	
}

//交叉运算过程 

void cross_change(int a,int b) {
	//生成交叉点 
	int cross_seed = random(4) + 1;
	cout<<"cross position:"<<cross_seed<<endl; 
	//输出交换前串信息 
	cout<<"temp["<<a + 1<<"]: ";

	for(int i = 0; i < 6; i++) {
		if(i == cross_seed) cout<<"| ";
		cout<<temp[a].bin[i]<<" ";		
	}
	cout<<endl;
	cout<<"temp["<<b + 1<<"]: ";

	for(int i = 0; i < 6; i++) {
		if(i == cross_seed) cout<<"| ";	
		cout<<temp[b].bin[i]<<" ";		
	}
	cout<<endl;

	//进行交换 
	for(int i = cross_seed; i < 6; i++) {
		int tmp;
		tmp = temp[a].bin[i];
		temp[a].bin[i] = temp[b].bin[i];
		temp[b].bin[i] = tmp; 
	}
} 

//变异运算 
void variation() {
	cout<<"Variation:"<<endl;
	for(int i = 0; i < SAMPLE_NUM; i++) {
		cout<<"temp["<<i<<"]:"	;
		int position_seed = random(5);
		cout<<"variate pos: "<<position_seed<<",";
		int var_seed = random(100);

		if(var_seed < VARIATE_RATE) {
			cout<<" success";
			if(temp[i].bin[position_seed]) 
				temp[i].bin[position_seed] = 0;
			else
				temp[i].bin[position_seed] = 1;
		}
		cout<<endl;	
	}

    //输出变异结果 
	cout<<"variate result:"<<endl;

	for(int i = 0; i < SAMPLE_NUM; i++) {
		cout<<"temp["<<i + 1<<"]: ";
		for(int j = 0; j < 6; j++)
			cout<<temp[i].bin[j]<<" ";
		cout<<endl;	 
	}
	cout<<endl;
}

//将运算的temp数组赋值给group数组以方便下一次迭代运算 
void trans() {
	for(int i = 0; i < SAMPLE_NUM; i++)
		group[i] = temp[i];
}

//输出信息 
void output() {	
	for(int i = 0; i < SAMPLE_NUM; i++) {
		cout<<"node["<<i + 1<<"]: ";
		for(int j = 0; j < 6; j++)
			cout<<group[i].bin[j]<<" ";
		cout<<" x1: "<<group[i].x1;
		cout<<" x2: "<<group[i].x2;
		cout<<" f: "<<group[i].f;
		cout<<" percent:"<<group[i].pct<<endl;
	}
	cout<<endl;
} 

//总函数接口 
void evolution() {
	init();
	calcu(); 
	output();
	for(int i = 0; i < EVOLVE_NUM; i++) {
		choose();
		cross(); 
		variation();
		trans();
		calcu(); 
		output();		
	}
}

int main() {
	srand((int)time(0));
	evolution();
}
```

结果：
![](http://img.blog.csdn.net/20141008123929000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbjk0MzQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
该函数最大值为98，有7个样本进化到了该数值，还有1个样本为次大值，效果不错。