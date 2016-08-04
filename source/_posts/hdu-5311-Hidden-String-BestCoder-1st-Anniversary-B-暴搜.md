title: hdu 5311 Hidden String BestCoder 1st Anniversary B 暴搜
tags: [搜索,暴力]
categories: 算法
date: 2015-07-26 17:10:00
---

## 题目

题目地址：[http://acm.hdu.edu.cn/showproblem.php?pid=5311](http://acm.hdu.edu.cn/showproblem.php?pid=5311)

## 题意

指定串为"anniversary"，在输入串中找到不相交的三段，拼起来数指定串，`len(s)`范围为100

## 分析

这题卡了挺久主要是题意理解问题，一直以为这三段不需要顺序拼接，例如`sary1ver1anni`这种应该页输出`YES`，然而并不是。事实上是简单的顺序拼接，这样的话可以进行最最暴力的for循环搜索了，搜索过程可以用KMP加速以下，而KMP的起始位置是动态变化的就行了。

## 代码

    #include <cstdio>
        #include <algorithm>
        #include <cstring>
        #include <vector>
        #include <map>
        #include <string>
        #include <iostream>
        using namespace std;
        void getNext(const std::string &p, std::vector<int> &next)
        {
            next.resize(p.size());
            next[0] = -1;

            int i = 0, j = -1;

            while (i != p.size() - 1)
            {
                //这里注意，i==0的时候实际上求的是next[1]的值，以此类推
                if (j == -1 || p[i] == p[j])
                {
                    ++i;
                    ++j;
                    next[i] = j;
                }
                else
                {
                    j = next[j];
                }
            }
        }

        int kmp(const std::string& s, const std::string& p, const int sIndex = 0)
        {
            std::vector<int>next(p.size());
            getNext(p, next);//获取next数组，保存到vector中

            int i = sIndex, j = 0;
            while(i != s.length() && j != p.length())
            {
                if (j == -1 || s[i] == p[j])
                {
                    ++i;
                    ++j;
                }
                else
                {
                    j = next[j];
                }
            }

            return j == p.length() ? i - j: -1;
        }
        string s;
        int main() {
            int T;
            scanf("%d", &T);
            while (T--) {
                cin >> s;
                string s0 = "anniversary";
                int len0 = s0.size();
                string s1, s2, s3;
                bool flag = false;
                for (int i = 1; i < len0; ++i) {
                    for (int j = i + 1; j < len0; ++j) {
                        s1.clear();
                        s2.clear();
                        s3.clear();
                        for (int t = 0; t < i; ++t) {
                            s1 += s0[t];
                        }
                        for (int t = i; t < j; ++t) {
                            s2 += s0[t];
                        }
                        for (int t = j; t < len0; ++t) {
                            s3 += s0[t];
                        }
                        // cout << s1 << " " << s2 << " " << s3 << endl;
                        int idx = kmp(s, s1);
                        if (idx == -1) {
                            continue;
                        } 

                        //printf("%d %d   111\n", i, j);
                        idx = kmp(s, s2, idx + s1.size());
                        if (idx == -1) {
                            continue;
                        }
                        //printf("222\n");
                        idx = kmp(s, s3, idx + s2.size());
                        if (idx == -1) {
                            continue;
                        }
                        flag= true;
                    }
                }
                if (flag) {
                    printf("YES\n");
                }
                else {
                    printf("NO\n");
                }
            }
            return 0;
        }
    