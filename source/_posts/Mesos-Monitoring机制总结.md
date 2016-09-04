title: Mesos Monitoring机制总结
date: 2015-12-23 22:24:56
tags: [mesos,监控]
categories: 基础架构
---
Mesos的master和slave提供了一组统计数据和指标，使用户能够方便地监控资源使用情况并且尽早发现异常状态。Mesos的监控包括可用资源，已经使用的资源，已经注册的框架，活跃的slave和任务的状态等。开发者可以利用这些监控信息实现自动化的报警，也可以在监控界面里绘制动态的监控图表。

<!-- more -->

## 指标

Mesos提供了两种不同的指标：`counter`和`gauge`。

`counter`（计数器）用来跟踪不连续的事件。它是单向递增的，并且它的类型是自然数。典型的例子就是失败的任务数量、注册的slave数量等。对于这类指标，变化率比数值本身更加重要。

`gauge`(我也不知道怎么翻译好）代表一瞬间的某个属性的测量值。例如当前集群中使用内存的总量，在线的slave连接数量。这类指标通常用来监控其是否在一段时间内高于或低于一个阈值（即是否异常）。

## Master

### api

Master节点的指标对外提供了api，具体可通过浏览器访问
```
http://<master-ip>:5050/metrics/snapshot
```
来进行获取，访问该链接(GET请求）会返回一组JSON串，内容为指标名称和内容的键值对。

### 1. 指标内容：Slave总资源

属于该类别的指标主要包括整个集群的可用资源总量和当前已经使用的资源部分。如果资源使用量持续高举不下，说明应该增加集群的资源总量，或者集群内的某个Framework已经运行失常。

|指标| 说明| 类型|
|----|----|----|
|`master/cpus_percent`|已使用的CPU占总量百分比|Gauge|
|`master/cpus_used`|已使用的CPU的个数|Gauge|
|`master/cpus_total`|CPU的总数|Gauge|
|`master/cpus_revocable_percent`|可回收|Gauge|
|`master/cpus_revocable_total`|Number of revocable CPUs|Gauge|
|`master/cpus_revocable_used`|Number of allocated revocable CPUs|Gauge|
|`master/disk_percent`|已使用的磁盘空间占总量百分比|Gauge|
|`master/disk_used`|已使用的磁盘空间(MB为单位)|Gauge|
|`master/disk_used`|已使用的磁盘空间(MB为单位)|Gauge|
|`master/disk_total`|磁盘总空间(MB为单位)|Gauge|
|`master/disk_revocable_percent`|	Percentage of allocated revocable disk space|Gauge|
|`master/disk_revocable_total`|Revocable disk space in MB|	Gauge|
|`master/disk_revocable_used`|Allocated revocable disk space in MB|Gauge|
|`master/mem_percent`|已使用的内存占总量百分比|Gauge|
|`master/mem_used`|已使用的内存（MB为单位）|Gauge|
|`master/mem_total`|内存总量（MB为单位）|Gauge|
|`master/mem_revocable_percent`|Percentage of allocated revocable memory	|Gauge|
|`master/mem_revocable_total`|Revocable memory in MB|Gauge|
|`master/mem_revocable_used`|Allocated revocable memory in MB|Gauge|


### 2. 指标内容：Master状态

这部分的指标主要说明Master是否是leading master，它已经启动运行了多长时间。当一个集群经过一段时间依然没有选出leading master，则认为这个集群已经处于故障状态。这说明要么竞选过程出现问题（检查 ZooKeeper 的连接），要么选出的 Master 有故障。此外较短的已经运行时间表明Master近期有过重启。

|指标| 说明| 类型|
|----|----|----|
|`master/elected`|是否是 leading master|Gaude|
|`master/uptime_secs`|已经运行时间（秒）|Gaude|

### 3. 指标内容：Master资源

这部分的指标主要说明指定Master的资源总量和使用情况。Master节点资源使用率长期高居不下，可能会影响集群的整体性能。区别于1中的资源，以下的system的资源特指Master的资源，而1中为Slave的总体资源情况

|指标| 说明| 类型|
|----|----|----|
|`system/cpus_total`|在这个master的可用 CPU 的数量 |Guage|
|`system/load_15min`|过去15分钟的平均负载|Guage|
|`system/load_5min `| 过去5分钟的平均负载|Guage|
|`system/load_1min`|过去1分钟的平均负载|Guage|
|`system/mem_free_bytes`|空闲内存的字节数|Guage|
|`system/mem_total_bytes`|全部内存的字节数|Guage|

### 4. 指标内容：Slave状态

改部分的指标主要包括Slave的事件、Slave数量和Slave状态的信息。活跃的Slave数量过少表明这个集群的Slave不健康，或者它们无法连接到当前的leading master。

|指标| 说明| 类型|
|----|----|----|
|`master/slave_registrations`|能够顺利地完成与Master断开然后                                                             重联的Slave的数量|Counter|
|`master/slave_removals`|各种原因被删除的Slave数量，包括维护|Counter|
|`master/slave_reregistrations`|重新注册的 slave 的数目|                                         Counter|
|`master/slave_shutdowns_scheduled`|因为健康检查失败而被计划删除                                                             的 slave 的数量。由于对删除 slave 的速度有限制（Slave Removal Rate-Limit），所以它们不一定会立即被删除。                                                             当他们真正被删掉，指标 master/slave_shutdowns_completed 会增加。|Counter|
|`master/slave_shutdowns_cancelled`|被取消的删除 slave 操作的数量。                                                               发生在因为删除 Slave 有限速，slave 在被删除之前有机会成功重连并且发送 PONG 给 master|Counter|
|`master/slave_shutdowns_completed`|成功地被删除了的 Slave 的数量|Counter|
|`master/slaves_active`|活跃的 slave 的数量|Guage|
|`master/slaves_connected`|连接在线的 slave 的数量|Guage|
|`master/slaves_disconnected`|断开离线的 slave 的数量|Guage|
|`master/slaves_inactive`|不活跃的 slave 的数量|Guage|
