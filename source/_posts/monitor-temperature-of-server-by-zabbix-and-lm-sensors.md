title: Zabbix配合lm_sensors监控服务器CPU温度
date: 2016-09-01 14:18:06
tags: [zabbix,lm_sensors,cpu]
categories: 系统运维
thumbnail: http://cdn.jetmuffin.com/posts/zabbix.png
---
手上的一个任务，需要对集群的每台机器 CPU 温度进行监控，防止集群在进行计算时温度过高导致 CPU 损坏。由于集群里已经部署了 Zabbix 进行监控和报警，所以很自然的想法就是在 Zabbix 中集成 CPU 监控这一内容。

首先是获取 CPU 温度的方法，常见的有两种：
 
 - IPMI
 - lm_sensors

其中 IPMI 需要硬件上的支持，同时还需要进行配置，比较复杂。而 lm_sensors 是 linux 下的一个小工具，安装简单，所以这里就选择 lm_sensors 来采集 CPU 的温度。
<!-- more -->

## 安装 

lm_sensors 的安装很简单，repo 里已经有这个包了，所以直接通过 yum 安装就行了（集群是用的 CentOS，Debian系的也差不多）。

```bash
$ yum install lm_sensors 
```

考虑到大多数集群是无法直接通外网的，所以可以在一台可以通外网的机器上下好 rpm 包，再传到 NFS 上或者直接 `scp`进去。下载 rpm 包可以使用 `yumdownloader`，刚好 lm_sensors 没有要依赖的包。

```bash
$ yumdownloader install lm_sensors
```

## 采集

采集数据之前，先看一下 zabbix 的自定义监控项的定义方法。在 `zabbix_agentd` 中对自定义监控项的定义格式为：

```
UserParameter=<key>,<shell command>
```

其中可以看出监控项通过一句 shell 命令获得，并且获得的需要是一个数值。这样我们大概明确了需要采集的目标。

然后先简单过掉下 lm_sensors 需要的配置，即监测 sensors 的信息。

```bash
$ sensors-detect
This program will help you determine which kernel modules you need
to load to use lm_sensors most effectively. It is generally safe
and recommended to accept the default answers to all questions,
unless you know what you're doing.

Some south bridges, CPUs or memory controllers contain embedded sensors.
Do you want to scan for them? This is totally safe. (YES/no):
```

一路敲 `yes` 即可，然后执行 `sensors` 命令可以看到 CPU 的温度：

```bash
$ sensors
power_meter-acpi-0
Adapter: ACPI interface
power1:     18446.74 GW  (interval =   2.00 s)

coretemp-isa-0000
Adapter: ISA adapter
Physical id 0: +41.0°C  (high = +75.0°C, crit = +85.0°C)
Core 0:        +30.0°C  (high = +75.0°C, crit = +85.0°C)
Core 1:        +32.0°C  (high = +75.0°C, crit = +85.0°C)
Core 2:        +28.0°C  (high = +75.0°C, crit = +85.0°C)
Core 3:        +34.0°C  (high = +75.0°C, crit = +85.0°C)
Core 4:        +30.0°C  (high = +75.0°C, crit = +85.0°C)
Core 5:        +30.0°C  (high = +75.0°C, crit = +85.0°C)

coretemp-isa-0001
Adapter: ISA adapter
Physical id 1: +38.0°C  (high = +75.0°C, crit = +85.0°C)
Core 0:        +33.0°C  (high = +75.0°C, crit = +85.0°C)
Core 1:        +34.0°C  (high = +75.0°C, crit = +85.0°C)
Core 2:        +32.0°C  (high = +75.0°C, crit = +85.0°C)
Core 3:        +31.0°C  (high = +75.0°C, crit = +85.0°C)
Core 4:        +32.0°C  (high = +75.0°C, crit = +85.0°C)
Core 5:        +31.0°C  (high = +75.0°C, crit = +85.0°C)
```

命令显示的结果是每个核心的温度以及物理核的温度，最后要聚合到一个数值上。这里可以选择两个方向，一是对每个核心进行监控，这样较为复杂，但是细节明显；二是对每个核心取平均值，处理比较简单。这里我选择了后者，那么就需要从命令输出中获取到需要的信息了。这里就用 `grep`，`cut`，`awk` 乱处理一通了。

首先用 `grep` 拿到每个核心的温度行：

```bash
$ sensors | grep "Core"
Core 0:        +30.0°C  (high = +75.0°C, crit = +85.0°C)
Core 1:        +32.0°C  (high = +75.0°C, crit = +85.0°C)
Core 2:        +30.0°C  (high = +75.0°C, crit = +85.0°C)
Core 3:        +34.0°C  (high = +75.0°C, crit = +85.0°C)
Core 4:        +31.0°C  (high = +75.0°C, crit = +85.0°C)
Core 5:        +30.0°C  (high = +75.0°C, crit = +85.0°C)
Core 0:        +31.0°C  (high = +75.0°C, crit = +85.0°C)
Core 1:        +33.0°C  (high = +75.0°C, crit = +85.0°C)
Core 2:        +32.0°C  (high = +75.0°C, crit = +85.0°C)
Core 3:        +30.0°C  (high = +75.0°C, crit = +85.0°C)
Core 4:        +32.0°C  (high = +75.0°C, crit = +85.0°C)
Core 5:        +31.0°C  (high = +75.0°C, crit = +85.0°C)
```

然后用 `cut` 或者 `awk` 获取每行的温度数值。

```bash
$ sensors | grep "Core" | awk '{print $3}' | awk -F '.' '{print $1}' | awk -F '+' '{print $2}'
30
33
29
34
30
30
31
34
32
32
32
30
```

最后对这些输出按行取平均，再四舍五入到整数就行了（也可以不四舍五入）。

```bash
$ sensors | grep 'Core' | awk '{print $3}' | awk -F "." '{print $1}' | awk -F "+" '{print $2}' | \
awk '{sum+=$1;count+=1} END {print sum/count}' | awk -F. '{if(substr($2,1,1)>=5)$1+=1 ; print $1}'
31
```

## 集成

做完数据的采集工作，那么就要集成进 Zabbix 里了。首先需要在 zabbix_agentd 里定义这个监控项，在 `zabbix_agentd.conf` 里加入

```
UserParameter=system.cpu.temperature,/usr/bin/sensors | grep 'Core' | awk '{print $3}' | awk -F "." '{print $1}' | awk -F "+" '{print $2}' | awk  '{sum+=$1;count+=1} END {print sum/count}' | awk -F. '{if(substr($2,1,1)>=5)$1+=1 ; print $1}'
```

然后重启 agent 服务即可。

```bash
$ service zabbix_agentd restart
Shutting down zabbix_agentd:                               [  OK  ]
Starting zabbix_agentd:                                    [  OK  ]
```

回到 Zabbix 的界面，选择一个合适的模板，进到它的监控项中，点击新建监控项，然后如下图填入信息即可。注意这里的键值即上面填的 `UserParameters` 里的 `key`。

![add-new-metric](/uploads/images/2016/09/01/QQ20160901-0@2x.png)

然后对相应的主机应用这个模板，那么就可以获取到这台主机的 CPU 温度了。

![metric-data](/uploads/images/2016/09/01/QQ20160901-1@2x.png)

报警设置和普通监控项相同，如下：

![alert](/uploads/images/2016/09/01/QQ20160901-2@2x.png)

最后，如果对 Zabbix 增加了 grafana 的话，还可以在 grafana 里去显示 CPU 温度。

![grafana](/uploads/images/2016/09/01/QQ20160901-3@2x.png)