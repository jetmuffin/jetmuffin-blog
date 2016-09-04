title: 8个8通过加减乘除得到1000 深搜+剪枝 算法实现
tags: [dfs,剪枝]
date: 2014-09-22 10:56:52
categories: 算法题解
---

算法导论的作业，编程使得8个8通过加减乘除得到1000。该题类似24点，但数据规模远大于24点的算法。具体思路参考24点的算法，在编程之美中有提到过。

深搜，然后用map 进行对2个操作数的判重进行剪枝，但是该方法仍然会得到大量的重复算式，想了想用set进行筛选重复的expression，虽然稍微影响效率，但是也没事，最终能在20秒左右搜出结果。

<!--more-->

代码：
```cpp
#include <string>  
#include <cstdlib>  
#include <ctime>  
#include <cmath>
#include <map>
#include <set>
using namespace std;  
const double EPS = 1e-6;  
const int NUM = 8;  
const int RES = 1000;  
double A[NUM];  
string res_str[NUM];  
set<string> ans; 
set<string>::iterator it;
int times = 0;  

bool dfs(int n) {  
    // 退出条件  
    if (n==1) {  

        if (fabs(A[0]-RES)<EPS) {  
//            cout << res_str[0] << endl;
			ans.insert(res_str[0]);  
        }  
    }  

    double a, b;  
    string expa, expb;  
    map<int ,int> hash;
    hash.clear();
    for (int i=0; i<n; i++)  
        for (int j=i+1; j<n; j++) {                 
            times++;  

            // 保存状态（操作数i,j）  
            a = A[i];  
            b = A[j];  
            expa = res_str[i];  
            expb = res_str[j];  

  			//hash判重 
            if(hash[a] == b) continue;
            if(hash[b] == a) continue;
            hash[a] = b;

            // 改变状态  
            A[j] = A[n-1];  
            res_str[j] = res_str[n-1];  

            // + 
            A[i] = a+b;  
            res_str[i] = '(' + expa + '+' + expb + ')';  
            if (dfs(n-1))  
                return true;  

			// -    
            A[i] = a-b;  
            res_str[i] = '(' + expa + '-' + expb + ')';  
            if (dfs(n-1))  
                return true;

			// - 反方向  
            A[i] = b-a;  
            res_str[i] = '(' + expb + '-' + expa + ')';  
            if (dfs(n-1))  
                return true;  

            // *
            A[i] = a*b;  
            res_str[i] = '(' + expa + '*' + expb + ')';  
            if (dfs(n-1))  
                return true;  

        	// /
            if (b!=0) {  
                A[i] = a/b;  
                res_str[i] = '(' + expa + '/' + expb + ')';  
                if (dfs(n-1))  
                    return true;  
            }  

            // /反方向 
            if (a!=0) {  
                A[i] = b/a;  
                res_str[i] = '(' + expb + '/' + expa + ')';  
                if (dfs(n-1))  
                    return true;  
            }  
            // 恢复状态  
            A[i] = a;  
            A[j] = b;  
            res_str[i] = expa;  
            res_str[j] = expb;  
        }  
    return false;  
}  

int main() {  
    for (int i=0; i<NUM; i++) {  
        A[i] = 8; 
        char c[10];  
        sprintf(c,"%.0f",A[i]);  
        res_str[i] = c;  
    }  

    cout<<"start searching...."<<endl;
    clock_t start = clock();
	dfs(NUM);  
    for(it = ans.begin(); it != ans.end();it ++){
		cout<<*it<<endl;
	} 

    clock_t duration = clock() - start;  
    cout<< "found : " <<ans.size()<<" expressions!"<<endl;
    cout << "spend : "<<duration << " ms"<<endl;  
}
