title: Difference between Mesos and Kubernetes
date: 2016-10-24 11:09:27
tags: [mesos,kubernetes,docker]
categories: 基础架构
---

自 Docker 大火之后，Swarm，Mesos，Kubernetes 这类围绕 Docker 容器展开的集群开源管理项目也逐渐风起云涌。本文主要从多个方面整理当前主流的两个容器编排框架 Mesos 和 Kubernetes 的区别。

<!--more-->  

## 介绍

Apache Mesos 始于加州大学伯克利分校的一个项目，用来驱动 Twitter 的底层基础架构，并且在之后成为许多大公司，如 eBay 和 Airbnb 的重要工具。之后 Mesos 的作者之一 Ben Hindman 创立了商业化公司 Mesosphere，并为 Mesos 社区贡献了许多持续性的开发和支持工具。

而 Kubernetes 则是出自 Google 公司，基于他们在 Borg 上的经验总结，推出的极具“谷歌风格”的开源集群容器管理平台。Kubernetes 和 Borg 的开发和维护是由 Google 的同一个团队进行的。

## 社区人气

在资源和容器管理方面，大公司（诸如微软、BAT 等）会自己造内部的轮子进行使用，而规模较小的公司则采用开源项目或基于开源项目进行二次开发。而在社区上，大家对 Docker 容器编排技术的选择更趋向于哪些方面？

![rihe.png](/uploads/images/2016/10/24/1.png)

Caicloud 在 2016 年 2 月统计了包括 Kubernetes, Swarm, Compose, Mesos, Docker Machine, Hyper, Containerd 在内的7个项目在 Github 上的 fork 数和被 star 数等。

### fork

在 fork 上的数据，Kubernetes 以 3699 次遥遥领先于其他所有的编排框架，而 Mesos 仅有 738 次 fork。而在笔者写这篇文章时（2016 年 10 月 24 日），Kubernetes 的 fork 数已经超过了 5800 次，而 Mesos 大概在 1050 次 fork。

![fork.png](/uploads/images/2016/10/24/2.png)

### star

star 的数据上 Kubernetes 依然以 12888 颗星数雄踞榜首，而 Mesos 仅有 2056 次 star。而直到今天，Kubernetes 已经拿到了近 17800 颗 star，而 Mesos 的 star 数量仅在 2600 次。

![star.png](/uploads/images/2016/10/24/3.png)

从 fork 和 star 数量可以看出两个框架在社区中的受欢迎程度，Kubernetes 远大于 Mesos。当然，由于 Kubernetes 出自名门，其中自然也带有一些来自于对 Google 公司的影响，而 Mesos 则更显得稍微小众一些。当然，社区的支持度越大，对开源框架的贡献者则越多，那么这个框架的发展则越迅速。

## 设计理念

虽然同样被视为容器编排框架，但是 Kubernetes 和 Mesos 的设计理念完全不同。

## 
