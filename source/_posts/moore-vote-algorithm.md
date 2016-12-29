title: Moore vote algorithm 多数投票算法
date: 2016-09-04 20:24:16
tags: [moore vote algorithm]
categories: 算法分析
---

在刷 leetcode 时，发现一道题目 [Majority Element](https://leetcode.com/problems/majority-element/)，要求求出数组中出现元素超过 `n/2` 的元素。思考了许久也只能写出 `O(nlogn)` 的解法（即用 map 计数）。查阅资料后发现一个比较巧妙的算法 Moore vote algorithm，可以在 `O(n)` 时间和 `O(1)` 空间解决这个问题。

<!-- more -->

## 算法思想

算法的思想很简单，每次从数组中取一对数，如果相同就叠加，如果不同就一起删去，很明显最后留下的肯定是数量最多的一个数。再深入一步实现，只要用一个变量记录**标记元素**和**标记元素的出现次数**，然后找下一个元素，分以下情况：

- 如果下一个元素和标记元素相同，那么标记元素出现次数加1；
- 如果和标记元素不同，那么标记元素出现次数减1；
- 如果标记元素变为 0，那么取下一个元素作为标记元素，初始出现次数为1；

举个说明性的例子，数列 `[2,3,2,2,4,1,2]`，标记元素变量 `now`，出现次数 `count`

1. 初始状态（取第一个数）：`now:2`，`count:1`
2. 取第二个数 `3`，它和 `now` 不同，那么 `count--`，这时候 `count = 0`，那么拿下一个元素（第三个数）为标记元素 `now:2`，`count:1`
3. 取第四个数 `2`，它和 `now` 相同，那么 `count++`，状态变为 `now:2`，`count:2`
4. 取第五个数 `4`，它和 `now` 不同，那么 `count--`，状态变为 `now:2`，`count:1`
5. 重复以上过程直到结尾，最后 `now:2`，所以数量超过 `n/2` 的元素为 2

## 算法实现

为了更好实现上述过程，事实上程序的伪代码为：

```
For i from 0 to len(arr):
    if count == 0:
        now = arr[i]
        count = 1
    else:
        if now == arr[i]:
            count ++
        else:
            count --
```

即只要标记元素被删去了，那么访问的下一个元素就作为标记元素。

## 代码实现

```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int count = 0, now = 0;
        for(int i = 0 ; i < nums.size(); i++) {
            if(count == 0) {
                now = nums[i];
                count = 1;
            } else {
                if(now == nums[i]) {
                    count ++;
                } else {
                    count --;
                }
            }
        }
        return now;
    }
};
```

## 参考资料

- [算法效果演示](http://www.cs.utexas.edu/~moore/best-ideas/mjrty/)
