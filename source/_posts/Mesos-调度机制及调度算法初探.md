title: Mesos 调度机制及调度算法初探
date: 2015-11-23 20:59:17
tags: [mesos,资源调度,分布式系统]
categories: 云计算
---
再次分析Mesos的工作原理，将Mesos中出现的各个名词串联到一起。

<!-- more -->

## 前言

Mesos是Apache下的开源分布式资源管理框架，它被称为是分布式系统的内核,Mesos将集群的所有节点的数据统一进行管理，打造一个DCOS(Data Center Operation System)的概念。调度算法是Mesos最核心也是灵魂部分。

## 调度架构

### 1. 两层架构

**第一层，由Mesos将资源分配给框架；第二层，框架自己的调度器将资源分配给自己内部的任务。**两层框架可以使得Mesos不需要知道任务的资源需求量，不需要知道任务的类型和数据存储，只需要和框架进行交互就行了。

### 2. Resource Offer

Mesos中的调度机制被称为**“Resource Offer”**，采用了基于资源量的调度机制。在Mesos中，Slave直接将资源量（CPU和内存）汇报给Master，由master将资源量按照某种机制分配给Framework，其中，“某种机制”是`“Dominant Resource Fairness（DRF）”`，在后面会提到该内容。

Framework可以根据是否符合任务对资源的约束，选择接受或拒绝`offer`。一旦`offer`被接受，Framework将与Master协作调度任务，并在数据中心的相应Slave节点上运行任务。

两个使`Resource Offer`高效且具有鲁棒性的附加机制：

1. **filters机制：** 每次调度，`mesos-master`和`framework-scheduler`需要进行通信，如果过多的拒绝`offer`会带来额外的通信开销，因此mesos提供了`filters`机制，允许Framework只接收**资源量大于L的offer**（换句话说被过滤的offer不会发送到Framework)

2. **rescinds机制：** 如果某个Framework接受了某个`offer`，然而长时间没有为该`offer`分配任务，而占有该`offer`（占着xx不xx），Mesos会回收其`offer`，并将其分配给其他Framework

## 调度工作流程

![mesos-workflow](/uploads/images/2015/11/23/mesos-workflow.png)

如上图所示，整个资源调度的工作过程为：

1. `Slave`定期向`Master`汇报其机器上的资源情况（包括cpu,mem,disk,port等）。

2. `Master`上的模块`Allocator`根据`DRF`算法决定要讲资源分配给哪个`Framework`，并将资源以`offer`形式发送给分配的`Framework`。（一层调度）

3. `Framework`编写时约束了每个`Task`执行需要的资源（cpu,mem,disk等），Framework根据约束筛选分配到的`offer`，可以选择接受`offer`，或者拒绝`offer`。若拒绝，`offer`随即可以被给其他`Framework`。（二层调度）

4. 每个`Task`对应一个`offer`，也对应一个`Slave`，则该`Task`会被该`Slave`上的守护进程`Executor`执行。同时，为了**资源隔离**，Mesos为任务的执行提供了`Isolator`隔离器，为了保证资源不会被其他`Task`访问，可用的Isolator有`Linux Container`，`Docker`等。

5. 由于`Master`很有可能出现单点故障，因此需要设立多`Master`，利用`Zookeeper`容错，保证集群运行的一致性。

**补充：**Mesos提供了拓展模块，使得我们如果要修改Mesos的部分内容，可以不用修改源码重新编译，而在以上流程中的`Allocator（重写调度方法）`、`Isolator（重写资源隔离方法）`均可以进行拓展，此外我还发现了Slave上的`Estimator`也能进行重写拓展，具体内容之后补充。

## DRF算法

如何作出offer分配的决定是由资源分配模块`Allocator`实现的，该模块存在于Master之中。资源分配模块确定Framework接受offer的顺序，与此同时，确保在资源利用最大化的条件下公平地共享资源。

由于Mesos为跨数据中心调度资源并且是异构的资源需求时，资源分配相比普通调度将会更加困难。因此Mesos采用了`DRF（主导资源公平算法 Dominant Resource Fairness）`,该算法同样出自于UC Berkeley。DRF的目标是确保每一个用户，即Mesos中的Framework，在异质环境中能够接收到其最需资源的公平份额。

Framework拥有的全部资源类型份额中占最高百分比的就是Framework的`主导份额`。DRF算法会使用所有已注册的Framework来计算`主导份额`，以确保每个Framework能接收到其`主导资源`的`公平份额`。

Mesos源码中默认的Allocator，即`HierarchicalDRFAllocator`的位置在`$MESOS_HOME/src/master/allocator/mesos/hierarchical.hpp`，而DRF中对每个Framework排序的`Sorter`位于`$MESOS_HOME/src/master/allocator/sorter/drf/sorter.cpp`，可以查看其源码了解它的工作原理。

### 一个说明性的例子

考虑一个`9CPU，18GBRAM`的系统，拥有两个用户，其中用户A运行的任务的需求向量为`{1CPU, 4GB}`，用户B运行的任务的需求向量为`{3CPU，1GB}`，用户可以执行尽量多的任务来使用系统的资源。

在上述方案中，A的每个任务消耗总cpu的`1/9`和总内存的`2/9`，所以A的**dominant resource**是`内存`；B的每个任务消耗总cpu的`1/3`和总内存的`1/18`，所以B的**dominant resource**为`CPU`。DRF会均衡用户的**dominant shares**，执行3个用户A的任务，执行2个用户B的任务。三个用户A的任务总共消耗了{3CPU，12GB}，两个用户B的任务总共消耗了{6CPU，2GB}；在这个分配中，每一个用户的**dominant share**是相等的，用户A获得了2/3的RAM，而用户B获得了2/3的CPU。

以上的这个分配可以用如下方式计算出来：x和y分别是用户A和用户B的分配任务的数目，那么用户A消耗了{xCPU，4xGB}，用户B消耗了{3yCPU，yGB}，在图三中用户A和用户B消耗了同等dominant resource；用户A的dominant share为4x/18，用户B的dominant share为3y/9。所以DRF分配可以通过求解以下的优化问题来得到：

```
max(x,y)            	#(Maximize allocations)
	subject to
		x + 3y <= 9 		#(CPU constraint)
		4x + y <= 18 		#(Memory Constraint)
			2x/9 = y/3 	#(Equalize dominant shares)
```

最后解出`x=3`以及`y=2`，因而用户A获得{3CPU，12GB}，B得到{6CPU， 2GB}。

### DRF在Mesos中的一个例子

换个使用场景，假设有数据中心包含一个offer为9核CPU和18GB内存。Framework 1运行的任务需要（1核CPU、4GB内存），Framework 2运行的任务需要（3核CPU、2GB内存）。Framework 1的任务会消耗CPU总数的1/9、内存总数的2/9，Framework 1的主导资源是内存。同样，Framework 2的每个任务会CPU总数的1/3、内存总数的1/18，Framework 2的主导资源是CPU。

![DRF-Table](http://cdn3.infoqstatic.com/statics_s1_20151118-0143/resource/articles/analyse-mesos-part-04/zh/resources/0604003.jpg)

上面表中的每一行提供了以下信息：

* **Framework chosen：**收到最新offer的Framework。
* **Resource Shares：**给定时间内Framework接受的资源总数，包括CPU和内存，以占资源总量的比例表示。
* **Dominant Share（主导份额：**给定时间内Framework主导资源占总份额的比例，以占资源总量的比例表示。
* **Dominant Share %（主导份额百分比）：**给定时间内Framework主导资源占总份额的百分比，以占资源总量的百分比表示。
* **CPU Total Allocation：**给定时间内接受的所有Framework的总CPU资源。
* **RAM Total Allocation：**给定时间内接受的所有Framework的总内存资源。

最初，两个Framework的主导份额是0％，我们假设DRF首先选择的是Framework 2，当然我们也可以假设Framework 1，但是最终的结果是一样的。

1. Framework 2接收offer并运行任务，使其主导资源成为CPU，主导份额增加至33％。
2. 由于Framework 1的主导份额维持在0％，它接收offer并运行任务，主导份额的主导资源（内存）增加至22％。
3. 由于Framework 1仍具有较低的主导份额，它接收offer并运行任务，增 加其主导份额至44％。
4. 然后DRF将offer发送给Framework 2，因为它现在拥有更低的主导份额。
5. 该过程继续进行，直到由于缺乏可用资源，不能运行新的任务。在这种情况下，CPU资源已经饱和。
6. 然后该过程将使用一组新的offer重复进行。

**需要注意的是**：可以创建一个资源分配模块，使用加权的DRF使其偏向某个Framework或某组Framework。如前面所提到的，也可以创建一些自定义模块来提供组织特定的分配策略。

## 效率

* 细粒度分配资源（细到每个任务在哪个机器上跑）,这是最显而易见的好处，也是Mesos社区和Mesosphere经常津津乐道的。

![Elastic Sharing](http://cdn2.infoqstatic.com/statics_s2_20151118-0143/resource/articles/analyse-mesos-part-02/zh/resources/mesos-elastic-cea4da90b3c819bd96b3158da1a6f86b.jpg)

* 支持各类应用资源共同管理，不用担心各个应用的资源冲突。

![Mesos Frameworks](http://cdn2.infoqstatic.com/statics_s2_20151118-0143/resource/articles/analyse-mesos-part-02/zh/resources/mesos_frameworks.png)

## 缺陷

在众多文章中看到大家对Mesos缺陷评价的一些共同点:

* Mesos中的DRF调度算法过分的追求公平，没有考虑到实际的应用需求。

* Mesos采用了Resource Offer机制，这种调度机制面临着资源碎片问题，即：每个节点上的资源不可能全部被分配完，剩下的一点可能不足以让任何任务运行，这样，便产生了类似于操作系统中的内存碎片问题。

* 由于Mesos采用了双层调度机制，在实际调度时，将面临设计决策问题：第一层和第二层调度器分别实现哪几个调度机制，即：将大部分调度机制放到第一层调度器，还是第一层调度器仅支持简单的资源分配

