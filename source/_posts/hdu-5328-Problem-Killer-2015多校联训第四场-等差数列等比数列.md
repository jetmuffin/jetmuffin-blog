title: hdu 5328 Problem Killer 2015多校联训第四场 等差数列等比数列
tags: [多校联训,等差数列,等比数列]
categories: 算法题解
date: 2015-08-01 10:16:04
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=5328](http://acm.hdu.edu.cn/showproblem.php?pid=5328)

## 题意

给定一个序列，求其中等差数列或者等比数列的最大长度

## 分析

思路很明确，对于从第二个起的输入数字，计算与前一个数的差和比，然后向后比较计数。值得一提的是，这题的数据非常严格，有很多的坑。下面列举以下：

*   坑一：输入输出一定要scanf+printf，不然铁定超时
*   坑二：等比数列比值要用浮点数，浮点数相等比较要用精度，即`fabs(a-b)<eps`
*   坑三：用double不知为何TLE了好多发
*   坑四：精度太低会WA，1e-6才能过
*   坑五：精度过高同样会TLE

## 代码
```cpp
    #include <iostream>
    #include <cstdio>
    #include <cmath>
    const float eps = 0.000001;
    using namespace std;
    int main(){
        int t;
    //    cin>>t;
        scanf("%d",&t);
        double ap,gp;
        int nap,ngp;
        int max_nap,max_ngp;
        double fi,se,ne;
        int n;
        while(t--){
            scanf("%d",&n);
            n--;
            scanf("%lf",&fi);

            nap = 1;
            ap = -1;
            max_nap = 0;

            ngp = 1;
            gp = -1;
            max_ngp = 0;

            bool iaSecond = true,ibSecond = true;

            while(n--){
                scanf("%lf",&ne);

                //ap
                if(iaSecond){
                    iaSecond = false;
                    ap = ne - fi;
                    nap = 2;
                }else{
                    if(fabs(ap - (ne - se)) < eps){
                        nap++;
                    }else{
                        ap = ne-se;
                        max_nap = max_nap>nap?max_nap:nap;
                        nap = 2;
                    }
                }
                //gp
                if(ibSecond){
                    ibSecond = false;
                    gp = ne / fi;
                    ngp = 2;
                    // cout << ne/fi <<endl;
                }else{
                    // cout << ne/se <<endl;
                    // cout<<fabs(gp - (ne / se))<<endl;
                    if(fabs(gp - (ne / se)) < eps){
                        ngp++; 
                    }else{
                        gp = ne/se;
                        max_ngp = max_ngp>ngp?max_ngp:ngp;
                        ngp = 2;
                    }
                }

                se = ne;
            }

            max_nap = max_nap>nap?max_nap:nap;
            max_ngp = max_ngp>ngp?max_ngp:ngp;
            if(max_nap > max_ngp)
                printf("%d\n", max_nap);
            else
                printf("%d\n", max_ngp);
        }
    }
```