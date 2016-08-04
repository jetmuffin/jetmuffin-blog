title: 利用Docker和Marathon构建带数据分析的Web应用
date: 2015-11-16 19:33:33
tags: [docker,mesos,marathon]
categories: 云计算
---
利用搭建的Mesos、Marathon和Docker的环境，搭建一个带数据分析模块的Web应用。

<!-- more -->

## 架构

该应用是一个网上商店（mesosphere官网给的demo...)，是个虚构的在线商店，由(nginx+Redis)提供Web服务，同时通过ELK构建日志搜索部分。
整个应用的架构如下：

![architecture](/uploads/images/11/16/m-shop-architecture-final-800x532@2x.png)

整个应用的`系统`部分使用(Mesos+Marathon+Docker)构建，用的是之前我们用`vagrant`建好的集群。

## 方法

下面记录下整个应用的构建步骤：

**(以下操作均在`node1`中进行)**

首先从mesosphere官方的Github仓库clone下来所需要用到的配置和程序[mhausenblas/m-shop](https://github.com/mhausenblas/m-shop)。

```
$ git clone https://github.com/mhausenblas/m-shop
```

OK,然后我们要使用Marathon中的REST接口来处理之后的应用请求了

首先，查看所有的应用

```
$ curl http://node1:8080/v2/apps
{"apps":[{"id":"/webapp","cmd":"python -m SimpleHTTPServer","args":null,"user":null,"env":{},"instances":1,"cpus":0.1,"mem":16.0,"disk":0.0,"executor":"","constraints":[],"uris":[],"storeUrls":[],"ports":[10000],"requirePorts":false,"backoffSeconds":1,"backoffFactor":1.15,"maxLaunchDelaySeconds":3600,"container":null,"healthChecks":[],"dependencies":[],"upgradeStrategy":{"minimumHealthCapacity":1.0,"maximumOverCapacity":1.0},"labels":{},"acceptedResourceRoles":null,"version":"2015-11-15T11:55:42.437Z","versionInfo":{"lastScalingAt":"2015-11-15T11:55:42.437Z","lastConfigChangeAt":"2015-11-09T08:37:51.733Z"},"tasksStaged":0,"tasksRunning":1,"tasksHealthy":0,"tasksUnhealthy":0,"deployments":[]}]}[
```

然后把这个没用的`webapp`给DELETE掉

```
$ curl -X DELETE http://node1:8080/v2/apps/webapp
{"version":"2015-11-16T12:41:39.408Z","deploymentId":"e8cc87f5-916b-4cab-9266-f72bb9756a56"}
```

然后把`system group`里的`mesos-dns`和`sedi`服务给PUT上去

```
$ curl -X PUT "http://node1:8080/v2/groups" -d @m-shop/mesos-dns/system.json -H "Content-type: application/json"
{"version":"2015-11-16T13:09:13.545Z","deploymentId":"98c3f565-f1fc-4162-b560-9f20ca5
```




