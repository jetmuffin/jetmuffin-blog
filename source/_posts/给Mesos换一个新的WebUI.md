title: 给Mesos换一个新的WebUI
date: 2015-11-23 18:29:00
tags: [mesos,webui]
categories: 云计算
---
虽然Mesos已经给用户提供了一个webui(`http://master:5050`)，然而这个UI无法很直观地显示集群的资源状况，因此可以给Mesos换一个更帅气的UI！

<!-- more -->

## 依赖

由于项目需要`nodejs`和`npm`环境，所以最好先配置好`nodejs`环境，最好`node`版本高于4.0，不然可能会出现错误。

然后是构建项目需要的`gulp`，运行

```
sudo npm install -g gulp
```

就可以了

## 安装

这里使用的是[Capgemini](https://github.com/Capgemini) group写的开源UI，首先感谢一下作者们~

项目地址为[Capgemini/mesos-ui](https://github.com/Capgemini/mesos-ui)

首先把整个项目clone下来

```
git clone https://github.com/Capgemini/mesos-ui mesos-ui
```

然后安装相关的依赖

```
cd mesos-ui
sudo npm install
```

最后使用`gulp`对项目进行build就行了

```
gulp build
```

## 使用

Mesos配置web地址非常方便，只需要在启动Mesos时添加`webui_dir=your_webui_dir`就好了

```
./bin/mesos-master.sh --ip=127.0.0.1 --work_dir=/var/lib/mesos --webui_dir=your_webui_dir
```

如果你的Mesos是以service的形式存在（换句话说就是不用自己手动启），也可以这么搞`export MESOS_WEBUI_DIR=/your-path/mesos-ui/build/`，然后重启服务

```
sudo service mesos-master restart
```

也就OK了

然后可以登陆`127.0.0.1:5050`去看看新UI了~

![mesos-ui](/uploads/images/2015/11/23/mesos-ui.png)
