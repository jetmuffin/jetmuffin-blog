title: 使用slurm进行集群的资源管理（vagrant）
tags: [资源管理,集群]
categories: 分布式
date: 2016-05-30 09:42:48
---
## 简介
>SLURM 是一种可用于大型计算节点集群的高度可伸缩和容错的集群管理器和作业调度系统。SLURM 维护着一个待处理工作的队列并管理此工作的整体资源利用。它还以一种排他或非排他的方式管理可用的计算节点（取决于资源的需求）。最后，SLURM 将作业分发给一组已分配的节点来执行工作并监视平行作业至其完成。

<!-- more -->
slrum相当于一个集群管理器，可以伸缩至大型节点集群，容错好，而更重要的是他开源。而现在，slurm已经成为很多强大的超级计算机上使用的资源管理器。

## 架构
![slurm架构](http://www.ibm.com/developerworks/cn/linux/l-slurm-utility/figure2.gif)
slurm的架构和一般的集群管理架构相类似，顶部是一对冗余集群控制器，它充当计算集群的管理器并实现一个管理守护程序（`slurmctld`），`slurmctld`提供了对计算资源的监视，同时它将进入的作业映射到基本的计算资源。

此外，每个计算节点上有一个守护程序`slurmd`，`slurmd`负责管理在其上执行的节点包括运行的任务，来自控制器的工作，以及接受控制器的请求等。

## 安装
安装上，为了模拟集群的环境，需要使用多个节点，所以我们用`vagrant`创建多个虚拟机。`Vagrantfile`如下：

```
# -*- mode: ruby -*-
# vi: set ft=ruby :
Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"
  config.ssh.insert_key = false
  config.vm.define "master" do |node|
	node.vm.network "private_network", ip: "192.168.10.10"
	node.vm.hostname = "master"
	node.vm.provider "virtualbox" do |v|
	  v.memory = 2048
	  v.cpus = 2
	end
  end
  config.vm.define "compute" do |slave|
	slave.vm.network "private_network", ip: "192.168.10.11"
	slave.vm.hostname = "compute"
  end
end
```
分别使用`vagrant up master`和`vagrant up compute`启动控制节点和计算节点。

Ubuntu源里提供了munge，所以可以直接通过源进行安装。在控制节点和计算节点分别安装slurm包，这个包里包含了`slurmctld`和`slurmd`

```
$ sudo apt-get install slurm-llnl
```

由于计算节点和控制节点需要通信，而通信之间需要进行认证，所以这里需要生成key并启动认证服务。这里使用的是llnl的munge。

```
$ sudo create-munge-key
Generating a pseudo-random key using /dev/urandom complete.
```

然后启动munge的认证服务

```
$ sudo service munge start
```

接下来配置slurm的配置文件，然而笔者在使用时，slrum的配置工具[Slurm Configuration Tool](https://computing.llnl.gov/linux/slurm/configurator.html)失效了，所以可以去slurm的源码里复制一份[example](https://github.com/SchedMD/slurm/blob/master/etc/slurm.conf.example)。我的配置文件如下：

```
ClusterName=vagrant
ControlMachine=master
#ControlAddr=
#
SlurmUser=slurm
SlurmctldPort=6817
SlurmdPort=6818
AuthType=auth/munge
StateSaveLocation=/tmp
SlurmdSpoolDir=/tmp/slurmd
SwitchType=switch/none
MpiDefault=none
SlurmctldPidFile=/var/run/slurmctld.pid
SlurmdPidFile=/var/run/slurmd.pid
ProctrackType=proctrack/pgid
ReturnToService=0
#
# TIMERS
SlurmctldTimeout=300
SlurmdTimeout=300
InactiveLimit=0
MinJobAge=300
KillWait=30
Waittime=0
#
# SCHEDULING
SchedulerType=sched/backfill
SelectType=select/linear
FastSchedule=1
#
# LOGGING
SlurmctldDebug=3
SlurmdDebug=3
JobCompType=jobcomp/none
#
# ACCOUNTING
#
# COMPUTE NODES
NodeName=compute Procs=1 State=UNKNOWN
PartitionName=debug Nodes=compute Default=YES MaxTime=INFINITE State=UP
```
这份配置文件在计算节点上同样要使用，所以要复制到计算节点上。

```
$ sudo scp /etc/slurm-llnl/slurm.conf vagrant@compute/~
```

然后启动控制节点上的slurmctld服务：

```
$ sudo service slurm-llnl start
* Starting slurm central management daemon slurmctld
```

接着是计算节点的配置，同样需要使用munge，将控制节点生成的`munge.key`拷贝到计算节点上。

```
$ sudo scp /etc/munge/munge.key vagrant@compute:/~
```

然后进入计算节点，将`munge.key`拷贝到munge的目录并启动munge,**注意要修改key的owner和group为munge**。

```
$ sudo mv ~/munge.key /etc/munge/
$ sudo chown munge:munge /etc/munge/munge.key
```

然后就可以启动munge服务了

```
$ sudo service munge start
```

再将之前复制的`slurm.conf`放到指定位置：

```
$ mv ~/slurm.conf /etc/slurm-llnl/slurm.conf
```
接着启动计算节点上的slurmd

```
$ sudo service slurm-llnl start
```

这样，slurm就配置完毕了。

## 使用

这里就简单测试下slurm的功能（在控制节点上运行），首先运行`sinfo`查看集群状况。

```
$ sinfo
PARTITION AVAIL  TIMELIMIT  NODES  STATE NODELIST
debug*       up   infinite      1   idle compute
```

然后启动一个任务（`/bin/hostname`）：

```
$ srun -N1 /bin/hostname
compute
```

## 错误与解决方案
**1. 启动munge报错误munged: Error: Logfile is insecure: group-writable permissions set on "/var/log"**

在Ubuntu 14.04下启动munge会出现以下错误：

```
$ sudo service munge start
 * Starting MUNGE munged [fail]
munged: Error: Logfile is insecure: group-writable permissions set on "/var/log"
```

原因是因为Ubuntu 14.04将`/var/log`目录的权限从0755 root:root调整为0755 root:syslog，因此直接启动会报group-writable permissions的错误。

解决方案是启动munge时加上`--force`或`--syslog`的参数。

* `--force`会将errors转为warnings，跳过错误
* `--syslog`会跳过`/var/log`目录的权限检查

如果是使用`/etc/init.d/munge start`或者`service munge start`的话，可以将Option写到配置文件`/etc/default/munge`里：

```
OPTIONS="--force"
```