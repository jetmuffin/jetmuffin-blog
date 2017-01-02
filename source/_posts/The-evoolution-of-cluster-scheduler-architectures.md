title: 集群调度架构的发展
date: 2017-01-02 20:41:33
tags: [资源调度,mesos,borg,omega,sparrow]
categories: 资源调度
---
文章翻译自[《The evolution of cluster scheduler architectures》](http://firmament.io/blog/scheduler-architectures.html)，原作者为 Cambridge 的 Sys@Scale 实验室。

<!--more-->

> 集群调度器作为当今基础设施架构中重要的组件，在近几年中发展非常迅速。它们的结构从整体化的设计逐渐松散化，分布式化，变得更为灵活。然而，许多现在的开源实现要么仍然采用整体化设计，要么缺少重要的功能，而这些功能则正是关系到现实世界的用户所关心的问题，即他们要求实现的高利用。

这篇文章是我们撰写的一个系列中的第一篇。这个系列阐述关于如何在大型集群调度任务，像那些互联网公司如 Amazon，Google，Facebook，Microsoft，Yahoo!等所在生产实践中所做的那样。调度是一个重要的主题，因为它直接影响着操作一个集群的代价。例如一个差的调度器会导致集群的使用率低下，这就意味着昂贵的服务器大部分在空闲着，必然造成经济上的损失。然而集群高使用率不仅仅是针对单个任务而言，因为任务之间往往会相互影响，这也就要求调度器在决策上更加地细致，小心。

## 架构发展

这篇文章讨论调度器的结构在近几年来是如何发展的，以及为什么这么发展。如图一所示，当前的调度器主要有以下几种不同的调度模式。其中灰色的正方形表示机器节点，彩色的圆形表示任务，而带有“S”的圆角矩形则代表调度器。箭头显示了调度器的调度决策，而三种不同的颜色（红黄蓝）则表示了不同的任务类型（例如 web 服务，批处理分析以及机器学习任务）。

![图1](http://cdn.jetmuffin.com/posts/scheduler-arch-sharestate.png)

### 中央调度架构

许多集群调度器是**中央调度架构**的，例如大多数的高性能计算中的调度器，[Borg 调度器](#)，一些早期的 Hadoop 中的调度器以及现在 Kubernetes 中的调度器。这种模式会在集群的某台机器上允许一个单独的调度进程（例如 Hadoop v1 中的 `JobTracker` 和 Kubernetes 中的 `kube-scheduler`），而这个调度进程负责分配任务到每台机器节点上。所有任务都是由一个调度器处理，也就是说所有的任务均是由一个调度逻辑进行分配的（如图 1a 所示）。这种架构简单一致，在许多越来越复杂的调度器中采用。例如 [Paragon](#) 和 [Quasar](#)，使用机器学习的方法来防止任务之间的资源竞争的调度器，也使用了这种调度模式。

相比于早些时候的 Hadoop 上的 MapReduce 任务，当前的集群允许的应用种类越来越多，不再针对单一的任务。然而，维护一个中央调度器来处理多类型混合任务是相当棘手的，原因有以下几点：

  1. 很明显调度器在应该用不同方式处理长时间服务作业（Long-running service jobs）和批处理作业（batch jobs）。
  2. 不同的应用有不同的需求，需要满足他们所有的需求相当于不断在调度器上增加功能，这也使得调度器的逻辑和实现越来越复杂。
  3. 调度器处理任务的顺序也会成为一个问题，队列的影响（例如 head-of-line blocking）和任务存储将难以处理，除非调度器是精心设计的。
  
总之，这些问题将造成工程上的噩梦，调度器的维护者将收到源源不断的功能需求而没法实现它。

### 两层调度架构

**两层调度架构**通过分离*资源分配*和*任务调度*这两部分来解决了这个问题。它上层允许针对不同的应用定制不同的任务分配逻辑，同时维持他们共用集群的能力。[Mesos](#)最先使用了这种架构，[YARN](#)也在某个版本之后开始支持。在 Mesos 中，底层资源管理器将资源以 `offer` 的形式提供给应用级的调度器，这些调度器可以在其中选择适合的 `offer` 并使用它来运行任务。而 YARN 中则允许应用级的调度器向底层资源管理器请求资源。图 1b 显示了这种架构模式：针对任务定制的调度器（S0-S2）和资源管理器进行交互，动态地对资源进行分片，分配给各个任务。总体而言，这是一种灵活的，允许根据任务类型进行定制的架构模式。

但是，两层调度框架分层的思想也存在着缺陷。应用层的调度器无法获得上帝视角，例如他们无法看到*所有*可能的调度选项。相反，他们仅仅能够看到通过资源管理器提供的资源对应的调度选项。这就存在着一些缺点：

  1. 优先级将难以实现：在基于 `offer` 的调度模型（Mesos）中，上层调度器无法看到分配给正在运行的任务的资源；在基于请求（YARN）的调度模型中，底层的资源管理器必须知道优先级策略（这和两层调度模式相矛盾）。
  2. 因为调度器无法看到所有的资源，它无法确认运行任务是否会影响降低资源的质量。
  3. 根据应用定制的调度器可能需要关注来自下层的资源中的许多不同的方面的信息，而他们仅仅能通过和底层资源管理器的接口获知这些内容，这也就导致这些接口可能会变得十分复杂。
  
### 共享状态架构

**共享状态架构**采用了一种半分布式的模型来解决上述的这些问题。在这个架构中，每个应用层的调度器相互独立地更新集群状态的多个副本，如图 1c 中所示。在这些变化在本地进行以后，调度器会进行启动一个乐观一致的事物去更新共享集群状态。当然这个事物也会失败，不同的调度器在同一时间进行了进行的变动可能会存在矛盾。

共享状态架构最典型的设计例子就是 Google 的 [Omega](#) 了，其次就是微软的 [Apollo](#)，以及 Hashicorp 的 [Nomad] 容器调度器。这些调度器都是将集群的共享状态储存在单个位置上，例如 Omega 中的 `cell state`，Apollo 中的 `resource monitor` 以及 Nomad 中的 `plan queue`。区别于其他两个调度框架，Apollo 中共享状态是只读的，同时调度事务是直接通过集群机器提交的。这些机器会自己确认是否存在矛盾，以及接受和拒绝这些变动。这也使得 Apollo 在集群共享状态暂时不可访问时也能短暂工作。

逻辑上的共享状态架构设计允许不在整个集群上实现完全的状态共享。在这种设计中（类似 Apollo 的设计），每台机器维护它自己的状态，发送要更新的状态到不同的 agent 上，例如调度器，机器的健康监控器，以及资源监控系统。每台机器局部的共享状态构成了整个集群的全局共享状态。

然而共享状态架构同样也有缺陷。和中央调度不同的是，共享状态总是处理着旧的集群状态信息，而不是实时的信息。这可能在高并发的场景下会降低调度器的性能。

### 完全分布式架构

**完全分布式架构**则将结构分解的更加彻底，如图 1d 所示，它甚至在调度器中没有协调者，仅仅是使用各个独立的调度器来对任务提供调度服务。这种架构中，每个调度器只是处理它本地的，部分的，常常过时的集群信息。作业可以提交到任意的调度器上，每个调度器可以调配任务到集群中的任意一个位置。和两层调度架构不同的是，它的调度器不负责对集群资源进行分片。相反，所有的调度和资源分片是任务在统计学上的多元性和随机性以及调度器的决策形成的结果。

虽然在之前有关于相关概念（利用多元随机决策）[在1996年发表](#)，最近的关于完全分布式架构的调度器的文章大概是从 [Sparrow](#) 的文章开始的。Sparrow 的主要前提是基于一个假设，即我们在集群中运行的任务在执行时间上在不断缩短，因为任务的划分粒度变得更加细粒度了。因此，作者假设瞬时任务数量变得更加庞大，这也就意味着调度器的决策吞吐量将会变得很大。而单个调度器可能支撑不了这么大的吞吐量（可能达到每秒百万个任务），Sparrow 想要将负载分配到多个调度器上。

这可能带来一些完美的作用，从概念上可以显示省去了中央的控制，同时它非常满足一些任务的需求——在之后的文章中会提到。在这里，以下一些原因足以显示这种分布式的，无中心协调的调度模式，在逻辑上相比比中央控制的，两层调度的，或共享状态的调度器，过于简单。例如：

 1. 分布式的调度器通常基于简单的 `"slot"` 概念，把每台机器细分成 n 个一致的 slot，在其中运行 n 个并行的任务。这简化了任务的资源需求不一致的问题，然而在现实中这个不一致的问题是存在的。
 2. 它同样会在 worker 上使用一些特定的队列（例如 Sparrow 中的 FIFO），这会限制调度的灵活性，因为这种架构下的调度器只能从集群中选择一台机器来讲一个任务入队。
 3. 因为分布式调度器没有中心控制，它在使用全局约束上回存在问题（例如要求公平性策略或者严格的优先级顺序时）
 4. 由于它是为了基于最少的信息来做快速的决策而设计的，分布式调度器难以支持复杂的或者应用指定的调度策略。换言之，这种调度目前能架构处理的任务相对比较简单。例如对相互影响的任务它在处理上将会十分棘手。
 
### 混合式架构

最近在学术上提出了**混合式架构**，为了解决完全分布式架构的问题，它将中央调度和共享状态调度设计相结合。它的工作方式是，将所有任务通过两种途径进行调度，对于短任务和低优先级批处理任务通过分布式调度来进行分配，而其他的任务则通过中央调度来分配，例如 [Tarcil](#)，[Mercury](#) 和 [Hawk](#)，图 1e 显示了这种设计模式的工作原理。这种设计的好处是，混合调度器混用了上述几种调度架构模式来避免各自的缺点。但据笔者所知，目前没有这种调度器用于生产实践中。

## 在实践中的结果

讨论不同调度架构之间相对的优缺点不仅仅是学术上的话题，同样需要在工业界生产实践中去考虑考虑诸如 Borg，Mesos 和 Omega 之类的文章。更进一步的是，许多上面讨论到的系统已经在许多大企业中投入生产使用，例如微软的 Apollo，谷歌的 Borg 和 Apple 的 Mesos，它们同样启示其他的系统可能作为开源项目在生产中使用也是可行的。

最近几年，许多集群允许容器华的任务，导致了许多不同的基于容器的“编排框架”开始出现。这也和谷歌以及其他公司声称的“集群管理者”相类似。然而，集群调度器上仍然有许多细节需要讨论，例如他们的设计原则，他们关注的调度 API等。更重要的是，许多用户既不知道调度器架构起到什么作用，也不知道这些调度器适合哪些应用。

图 2 显示了一系列开源的编排框架的概况，以及它们的架构和它们的调度器支持的功能。在表格的最底部，我们同样引用了谷歌和微软的非开源的系统。`Resource granularity` 这一列表示资源使用固定的 slots，还是用多维的形式分配资源。

![图2](http://cdn.jetmuffin.com/posts/orch-framework-feature-matrix.png)

其中一个帮助你确定调度框架是否合适的重要方面是你的集群是否允许不同的任务。例如在同一个集群中既有前端服务（负载均衡 web server 和 memcached）以及数据批处理分析（例如 MapReduce 或者 Spark）。在同一个集群上运行这两类任务可以提高该集群的使用率，但是不同的应用可能有不同的调度需求。在这种情况下，中央调度的调度器可能导致一个局部最优的调度结果，因为它在逻辑上无法做到根据每个任务进行的多样性调度。而两层调度和共享状态调度可能就更加适合一些。

大多数用户面对的服务任务是用服务时的峰值需求来设定的资源分配量的，但是在实际中通常这些服务的资源使用率是低于他们的分配需求。**在这种情况下，能够适当的超售资源给低优先级的任务（同时保证服务的 QoS）是集群效率的关键。**Mesos 是当前的开源系统中唯一支持资源超售的，而 Kubernetes 则是把它作为一个长远的目标。我们应该更加关注这个方面，因为根据谷歌 Borg 的报告指出，大多数集群的使用率仍然是低于 60-70% 的。我们将在未来的这个系列的文章中更加关注资源评估，资源超配和高效的机器使用率。

最后，特定的分析和 OLAP 风格的应用（例如 Dremel 或 SparkSQL queries）适用于完全分布式调度器。然而，完全分布式调度器（例如 Sparrow）受到一系列的约束，而在任务相同时工作效果最好。我们会在之后的文章中更加详细地解释完全分布式调度器和混合调度器中的分布式组件部分的内容。而现在足以说明从观察的结果看出，这种架构在调度策略上过于简单，以致于没法支持多类资源类型，资源超售和资源重分配等功能。

总体的说，从图 2 的表格中可以看出目前的开源调度框架还有很多的功能需要去支持和改进。这应该成为一个行动号召，解决这些框架中缺少的功能，低下的使用率，不可预测的任务性能等问题。

然而，一个好消息是，许多框架采用中央调度模式的调度框架正在往更加灵活的设计上发展。Kubernetes 已经支持了可插件化的调度器（`kube-scheduler` pod 可以被其他兼容 API 的调度器 pod 替代），从 v1.2 开始支持多调度器，以及正在开发的支持用户自定义的调度策略等。Docker Swarm 在未来可能同样会变得可插件化。

## 下回预告

本系列的下篇文章会关注完全分布式架构的主要问题。同时，我们会关注 resource-fitting 策略，以及最后讨论为什么我们的 Firmament scheduling 平台综合了其他调度架构所有的优点。

## 参考文献
1. Verma A, Pedrosa L, Korupolu M, et al. Large-scale cluster management at Google with Borg[C]//Proceedings of the Tenth European Conference on Computer Systems. ACM, 2015: 18.
2. Hindman B, Konwinski A, Zaharia M, et al. Mesos: A Platform for Fine-Grained Resource Sharing in the Data Center[C]//NSDI. 2011, 11: 22-22.
3. Vavilapalli V K, Murthy A C, Douglas C, et al. Apache hadoop yarn: Yet another resource negotiator[C]//Proceedings of the 4th annual Symposium on Cloud Computing. ACM, 2013: 5.
4. Boutin E, Ekanayake J, Lin W, et al. Apollo: scalable and coordinated scheduling for cloud-scale computing[C]//11th USENIX Symposium on Operating Systems Design and Implementation (OSDI 14). 2014: 285-300.
5. Schwarzkopf M, Konwinski A, Abd-El-Malek M, et al. Omega: flexible, scalable schedulers for large compute clusters[C]//Proceedings of the 8th ACM European Conference on Computer Systems. ACM, 2013: 351-364.
6. Ousterhout K, Wendell P, Zaharia M, et al. Sparrow: distributed, low latency scheduling[C]//Proceedings of the Twenty-Fourth ACM Symposium on Operating Systems Principles. ACM, 2013: 69-84.
7. Delimitrou C, Kozyrakis C. Paragon: QoS-aware scheduling for heterogeneous datacenters[C]//ACM SIGPLAN Notices. ACM, 2013, 48(4): 77-88.
8. Delimitrou C, Kozyrakis C. Quasar: resource-efficient and QoS-aware cluster management[C]//ACM SIGPLAN Notices. ACM, 2014, 49(4): 127-144.
9. Delimitrou C, Sanchez D, Kozyrakis C. Tarcil: Reconciling scheduling speed and quality in large shared clusters[C]//Proceedings of the Sixth ACM Symposium on Cloud Computing. ACM, 2015: 97-110.
10. Delgado P, Dinu F, Kermarrec A M, et al. Hawk: Hybrid datacenter scheduling[C]//2015 USENIX Annual Technical Conference (USENIX ATC 15). 2015: 499-510.
11. Karanasos K, Rao S, Curino C, et al. Mercury: Hybrid centralized and distributed scheduling in large shared clusters[C]//2015 USENIX Annual Technical Conference (USENIX ATC
