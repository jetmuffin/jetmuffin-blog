title: hdu 5319 Painter 2015多校联训第三场 暴力
tags: [多校联训,暴力]
categories: 算法
date: 2015-08-01 09:58:00
---

## 题目

题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=5319](http://acm.hdu.edu.cn/showproblem.php?pid=5319)

## 题意

每次R操作只能从`\`斜线方向画，B操作只能从`/`斜线方向画，给定最后状态，问最少得画几次才能达到。

## 分析

思路比较明确，只要进行逆运算，消除R，B，G就行了，消除的方法就从左下角往右上角依次进行R的消除（`'R'->'.','G'->'B'`)，再从左上角到右下角依次进行B的消除(`'B'->'.','G'->'R'`)，然后每次斜线消除若碰到相反的字母或'.'，就增加一次计数就行了

## 代码

    #include <iostream>
    #include <cstdio>
    #include <string>
    #include <cstring>
    using namespace std;

    char map[55][55];

    int main(){
        int T;
        cin>>T;
        int n,m;
        while(T--){
            cin>>n;
            for(int i=0;i<n;i++){
                scanf("%s",map[i]);
            }
            m = strlen(map[0]);
            int cnt = 0;
            for(int l=0;l<m;l++){
                int i=0,j=l;
                while(i<n && j<m){
                    if(map[i][j] != 'R' && map[i][j] != 'G'){
                        i++;
                        j++;
                    }else{
                        break;
                    }
                }
                int isR = 0;
                while(i<n && j<m){
                    if(map[i][j] == 'R' || map[i][j] == 'G'){
                        isR = 1;
                        if(map[i][j] == 'G')
                            map[i][j] = 'B';
                        i++;
                        j++;
                    }else{
                        if(isR){
                            cnt++;
                        }
                        isR = 0;
                        i++;
                        j++;
                        continue;
                    }
                }
                if(isR){
                    cnt++;
                }
            }
            for(int l=1;l<n;l++){
                int i=l,j=0;
                while(i<n && j<m){
                    if(map[i][j] != 'R' && map[i][j] != 'G'){
                        i++;
                        j++;
                    }else{
                        break;
                    }
                }
                int isR = 0;
                while(i<n && j<m){
                    if(map[i][j] == 'R' || map[i][j] == 'G'){
                        isR = 1;
                        if(map[i][j] == 'G')
                            map[i][j] = 'B';
                        i++;
                        j++;
                    }else{
                        if(isR){
                            cnt++;
                        }
                        isR = 0;
                        i++;
                        j++;
                        continue;
                    }
                }
                if(isR){
                    cnt++;
                }
            }
            for(int l=0;l<m;l++){
                int i=n-1,j=l;
                while(i>=0 && j<m){
                    if(map[i][j] != 'B'){
                        i--;
                        j++;
                    }else{
                        break;
                    }
                }
                int isR = 0;
                while(i>=0 && j<m){
                    if(map[i][j] == 'B'){
                        isR = 1;
                        i--;
                        j++;
                    }else{
                        if(isR){
                            cnt++;
                        }
                        isR = 0;
                        i--;
                        j++;
                        continue;
                    }
                }
                if(isR){
                    cnt++;
                }
            }
            for(int l=n-2;l>=0;l--){
                int i=l,j=0;
                while(i>=0 && j<m){
                    if(map[i][j] != 'B'){
                        i--;
                        j++;
                    }else{
                        break;
                    }
                }
                int isR = 0;
                while(i>=0 && j<m){
                    if(map[i][j] == 'B'){
                        isR = 1;
                        i--;
                        j++;
                    }else{
                        if(isR){
                            cnt++;
                        }
                        isR = 0;
                        i--;
                        j++;
                        continue;
                    }
                }
                if(isR){
                    cnt++;
                }
            }
            cout<<cnt<<endl;
        }
    }
    