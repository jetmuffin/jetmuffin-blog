title: 2014ACM-ICPC 地区赛鞍山站小结
tags: [ACM现场赛]
categories: 心路历程
date: 2014-10-21 08:05:45
---

<span style="white-space:pre"></span>第一次参加ACM地区赛，感受挺深，以下用流水帐的形式记录一下这次的经历。

<span style="white-space:pre"></span>从出发开始说起，学校给定的火车票，从南京到鞍山，23小时车程，早上5点从学校出发，也是不容易，上车后发现南航的就在隔壁，压力甚大。报道时没有教练在，都由我们自己处理，有一点印象比较深，当志愿者问我们来自什么学校时，回答“河海大学”让边上天津大学的教练惊讶了一番，不知是学校太不出名还是太弱的原因。。感慨ORZ。主办方辽宁科技大学很周到，一个队伍配了一个志愿者，从开始引导到结束，这点很好～XD。

<span style="white-space:pre"></span>第一天下午参加了热身赛，4道题目。一开始场地的交换机出问题了，不能交题不能看scoreboard，而且problem set三人只能有一份，我们只好每人一张开始切题。我开始看的是A题（不记得了），是要计算一个长度最大为28的全排列中符合条件（即任意一个数与前后两个数都互质）。出于范围比较小，全排列范围是28！，我想了一个搜索+剪枝方法，开始打代码，纪存和范仕良在看其他题。码完题测试下速度，发现当数据到达13的规模时时间超过1秒，果断放弃。这时候切出去看了一下scoreboard，发现好多人做出了D（不记得了），然后范仕良和我交流了一下思路，觉得可行，我花了2分钟打完代码，交了，1Y。随后看其他题，一道概率DP，实在太难找出递推公式，无奈放弃。时间过了2小时时看了一下scoreboad发现AC2道的队伍不超过10支，然后我们就开始很随意聊天了XD～比赛结束看了一下排名，97,铜牌最后一名，还是很不错的～

<!--more-->

<span style="white-space:pre"></span>吃完晚饭，闲着无聊我们队3人跑到附近的网吧休闲一番～发现有队伍居然在网吧打BC。。给大神跪了OTZ～

<span style="white-space:pre"></span>第二天早上正赛，赛前有点紧张，比赛时发现更紧张。开始时我切的是C，纪存看I，范仕良看的K（不记得了）。3分钟左右看一下排名，I有人出了，立马转战I，了解题意后还是由我主打代码，打完后稍微检查一番决定交，结果WA，当时就蒙了，再次审查一遍，认为是sqrt中double的问题，修改再交，依然WA。当时真是整个情绪有点崩溃，毕竟这只是道签到题。再次审查，把该改的地方再完善，依然WA。最后发现错误错在一个平时都不注意的地方。大概是：

<span style="white-space:pre"></span>
<pre code_snippet_id="490578" snippet_file_name="blog_20141021_1_3526745"  code_snippet_id="490578" snippet_file_name="blog_20141021_1_3526745" name="code" class="cpp">double dis = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)</pre>

其中x1,x2,y1,y2都是1e5的范围，当时我们一直觉得这样赋&#20540;，计算后的结果范围1e10肯定不会超double，但是居然是这儿出错了，原因是该语句先计算结果后转换精度，而结果爆int了。修改后AC，这时候看一下排行，120+，有些崩溃。

<span style="white-space:pre"></span>转战E题，我认为这题只要弄清楚各个情况即可，于是我开始打代码，写完发现测试数据都过不了。这时范仕良已经整理思路，换他来写了一个DP，1Y。

<span style="white-space:pre"></span>这时我们看了下排行，110+，而3题的大概只有30队。迅速看完D题，讨论，想出思路时大概50支队伍3题，赛后发现如果这时候1Y很有希望拿铜。

<span style="white-space:pre"></span>E题题意大概是有n个点，去掉k个点使得剩下的点方差最小。

<span style="white-space:pre"></span>一开始范仕良的思路是：最后得到的一定是个连续的区间，所以枚举各个长度为n-k的区间即可，但是被我和纪存否定了，原因是我们认为对于每次枚举要重新计算方差，整体复杂度为o(n*k)，会超时。之后纪存想出了一个方法，每次删去离重心最远的点，更新重心，复杂度o(n)，讨论后我们都觉得此方法可行，由我打代码，打完交后WA，再次找错误，改精度，依然WA，最后卡题到结束。没能拿到牌都挺郁闷。

<span style="white-space:pre"></span>返程的火车上和南理工的同学讨论了下，他们指出了错误。当一堆数据中有一部分密集无效数据和有效数据距离太远时，我们的算法结果是删后的数据重心在有效和无效数据之间，而正确的结果应该是删除无效的数据。之后我随机生成数据进行测试，发现错误所在。

<span style="white-space:pre"></span>以下为我们的错误代码：

<span style="white-space:pre"></span>

`#include <iostream>

#include <algorithm>

#include <cstring>

#include <cstdio>

double p[50005];

using namespace std;

int main()

{

int T;

cin>>T;

while(T--)

{

memset(p,0,sizeof(p));

int n,k;

cin>>n>>k;

double sum = 0;

for(int i = 1;i<=n;i++)

{

cin>>p[i];

sum+= p[i];

}

sort(p+1,p+n+1);

double avp = sum/(double)n;

int fst = 1;

int lst = n;

for(int i = 0;i < k; i++)

{

double fstv = avp - p[fst];

double lstv = p[lst] - avp;

cout << p[fst]<<" : "<<fstv << " "<<p[lst]<<" : "<<lstv<<endl;

if(fstv > lstv)

{

sum -= p[fst];

fst ++;

avp = sum/(double)(n-i-1);

}else{

sum -= p[lst];

lst --;

avp = sum/(double)(n-i-1);

}

}

double ans = 0;

for(int i=fst;i<=lst;i++)

{

cout<<p[i]<<" ";

}

cout<<endl;

for(int i=fst;i<=lst;i++)

{

ans += (p[i] - avp)*(p[i] - avp);

}

printf("%.10f\n",ans);

}

return 0;

}

</pre>

<span style="white-space:pre"></span>当该算法碰到如下数据时：

<span style="white-space:pre"></span>1

13 4

1 1 1 1 5 6 7 8 9 10 11 12 13

会将10 11 12 13删除得道错误答案90

而正确答案则是删去4个1得到60.

再次思考原范仕良的思路是对的，只要事先处理一下方差公式:

(xi - x平均)^2 = xi^2 + x平均^2 - 2*xi*x平均

所以方差 = ∑xi^2 + n*x平均 - 2*∑xi*x平均

事先计算∑xi^2和∑xi，便可以以o(n)扫一遍得出结果。

代码：

<code lang="cpp">#include <iostream>  

#include <algorithm>  

#include <cstring>  

#include <cstdio>  

double p[50005];  

using namespace std;  

int main()  

{  

    int T;  

    cin>>T;  

    while(T--)  

    {  

        memset(p,0,sizeof(p));  

        int n,k;  

        cin>>n>>k;  

        for(int i = 1;i<=n;i++)  

            cin>>p[i];  

        if(n==k)  

        {  

            printf("%.9f\n",0);   

            continue;      

        }   

        sort(p+1,p+n+1);  

        double sum = 0;  

        double sqrsum = 0;  

         for(int i = 1;i<=n-k;i++)  

         {  

                sum += p[i];  

                sqrsum += p[i] * p[i];  

         }  

        double avg  = sum/(double) (n-k);  

        double min = sqrsum + (n-k)*avg*avg - 2*sum*avg;  

       // cout<<sum<<" "<<sqrsum<<" "<<avg<<" "<<min<<endl;  

        int flag = 0;  

        for(int i = 1 ;i <= k; i++)  

        {  

                sum = sum + p[n-k+i] - p[i];  

                sqrsum = sqrsum + p[n-k+i]*p[n-k+i] - p[i]*p[i];  

                avg = sum/(double) (n-k);  

                double now  = sqrsum + (n-k)*avg*avg - 2*sum*avg;  

              //     cout<<i<<" "<<sum<<" "<<sqrsum<<" "<<avg<<" "<<min<<endl;  

                if(now < min)  

                {  

                    min = now;  

                    flag = i;  

                }  

        }  

        printf("%.10f\n",min);  

}  

}`

鞍山赛区失利，不过没有关系，西安争取拿牌～

最后感谢一下比赛期间一直引导我们的志愿者粘微微（姓很奇怪XD～）

文章流水帐，无所谓啦～