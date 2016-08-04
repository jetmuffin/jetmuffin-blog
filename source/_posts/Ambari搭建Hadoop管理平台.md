title: Ambari搭建Hadoop管理平台
date: 2015-10-19 10:32:15
tags: [云计算,hadoop]
categories: 云计算
---

## 介绍
Apache Ambari项目的目标是通过配置、管理、监控Hadoop集群，使得Hadoop集群的管理更加简单。Ambari提供了直观易用、基于Restful APIs的Web UI。
<!--more-->
Ambari使得系统管理员可以：

* 配置Hadoop集群
    * Ambari提供了逐步安装Hadoop服务的向导
    * Ambari处理了Hadoop集群中复杂的配置
* 管理Hadoop集群
    * Ambari对整个Hadoop集群提供了中心管理服务，可以进行Hadoop的启动、终止和重新配置。
* 监控Hadoop集群 
    * Ambari提供了监控Hadoop集群健康和状态的控制台
    * Ambari使用`Ambari Metrics System`进行各项指标的收集
    * Ambari使用`Ambari Alert Framework`进行系统报警（当节点宕机或者剩余空间不足等）

## 安装Ambari(Ubuntu 14.04)

### step1: 从服务器下载Ambari

```
cd /etc/apt/sources.list.d
wget http://public-repo-1.hortonworks.com/ambari/ubuntu14/2.x/updates/2.1.2/ambari.list
```

### step2: 安装、启动Ambari Server
从Ambari仓库下载Ambari Server：
```
apt-key adv --recv-keys --keyserver keyserver.ubuntu.com B9733A7A07513CAD
apt-get update
apt-get install ambari-server
```
执行命令配置Ambari Server, Database, JDK, LDAP，和其他配置项：
```
ambari-server setup
```
启动 Ambari Server：
```
ambari-server start
```

### step3: 使用Ambari Web UI
浏览器打开地址`http://<ambari-server-host>:8080`
使用账号`admin`和密码`admin`进行登录

## 配置环境

### 配置mysql数据库
```
mysql -u root -p
mysql> create database ambari;
mysql> use ambari;
Database changed
mysql>source /var/lib/ambari-server/resources/Ambari-DDL-MySQL-CREATE.sql
```

## 设置ambari-server
```
ambari-server setup
```
过程中会要求配置运行用户、JDK、数据库等内容

## 启动ambari-server
```
ambari-server start
```
**若启动失败，可以查看/var/log/ambari-server/ambari-ser.out**
对于以下错误：

> com.mysql.jdbc.exceptions.jdbc4.Communicationsxception: Communications link failure

可以采取以下方法：
```
vim /etc/mysql/my.cnf
# 将bind-address: 127.0.0.1 改为 bind-address: 0.0.0.0
/etc/init.d/mysql restart
ambari-server restart
```
