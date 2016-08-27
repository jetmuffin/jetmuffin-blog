title: 源码编译安装Ambari
date: 2015-10-22 13:18:06
tags: ambari,hadoop
categories: 基础架构
---
# 源码编译安装Ambari

---

## 为什么
笔者用源码编译的原因很简单，hortonworks的源太不稳定了，自己的几台Ubuntu的机子添加源后无法下载，且CentOS的机子添加源后无法下载ambari-agent，只能另辟蹊径使用源码编译

<!-- more -->

## 环境准备
源码编译需要以下环境，需要提前安装：
> * JDK
> * Maven
> * Python 2.6+
> * rpm-build
> * g++,gcc,make等
> * NodeJS
> * Brunch

因此需要先配置环境

### 安装JDK
可以使用系统(CentOS)自带的openjdk，或者下载Oracle的JDK。
安装后配置环境
```
[root@cloud01 ~]# vim /etc/profile
# 在末尾添加 export JAVA_HOME=你的java地址
# 在末尾添加 export path=$path:$JAVA_HOME/bin
[root@cloud01 ~]# source /etc/profile
[root@cloud01 ~]# java -version
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
```
如果`java -version`正常，则JDK安装成功

### 安装Maven
1. 官网下载maven对应版本
2. 解压

    ```
    [root@cloud01 ~]# tar -zxvf apache-maven-3.0.3-bin.tar.gz
    ```
    
3. 设置环境变量

    ```
    [root@cloud01 ~]# vim /etc/profile
    #末尾添加 export MAVEN_HOME=你的maven地址
    #末尾添加 export path=$path:$MAVEN_HOME/bin
    [root@cloud01 ~]# source /etc/profile
    [root@cloud01 ~]# mvn -v
    Apache Maven 3.0.3
    Maven home: /usr/local/apache-maven-3.0.3
    Java version: 1.7.0_51, vendor: Oracle Corporation
    Java home: /usr/local/jdk1.7.0_51/jre
    Default locale: en_US, platform encoding: UTF-8
    OS name: "linux", version: "2.6.32-358.el6.x86_64", arch: "amd64", family: "unix"
    ```
    
若 `mvn -v`正常，则maven安装成功

### 安装Python
CentOS自带Python，且CentOS6.5自带Python2.6，可以直接使用
若版本过低或者未安装则执行
```
[root@cloud01 ~]# yum install python
```

### 安装rpm-build
1. 检测rpm-build是否已经安装

    ```
    [root@cloud01 ~]# rpm -qa | grep rpm-build
    ```
    
2. 若未安装，执行

    ```
    [root@cloud01 ~]# yum install rpm-build
    ```

### 安装g++等依赖
执行
```
[root@cloud01 ~]# yum -y install gcc make gcc-c++ openssl-devel wget 
```

### 安装NodeJS
1. 下载源码及解压

    ```
    [root@cloud01 ~]# wget http://nodejs.org/dist/v0.10.26/node-v0.10.26.tar.gz
    [root@cloud01 ~]# tar -zvxf node-v0.10.26.tar.gz 
    ```

2. 编译及安装
   
    ```
    [root@cloud01 ~]# cd node-v0.10.26
    [root@cloud01 ~]# ./configuration
    [root@cloud01 ~]# make && make install
    ```
    
3. 验证安装配置是否成功

    ```
    node -v
    ```

## 编译
1. 下载并解压源码

    ```
    [root@cloud01 ~]# wget http://www.apache.org/dist/ambari/ambari-2.1.2/apache-ambari-2.1.2-src.tar.gz
    [root@cloud01 ~]# tar xfvz apache-ambari-2.1.2-src.tar.gz
    [root@cloud01 ~]# cd apache-ambari-2.1.2-src
    ```

2. 编译

    ```
    [root@cloud01 ~]# mvn versions:set -DnewVersion=2.1.2
    [root@cloud01 ~]# pushd ambari-metrics
    [root@cloud01 ~]# mvn versions:set -DnewVersion=2.1.2
    [root@cloud01 ~]# popd
    [root@cloud01 ~]# mvn -B clean install package rpm:rpm -DnewVersion=2.1.2 -DskipTests -Dpython.ver="python >= 2.6"
    ```

3. 安装rpm

    ```
    [root@cloud01 ~]# yum install ambari-server*.rpm
    ```
    
## 错误总结
1. 出现异常：ClassNotFoundException: org.slf4j.helpers.MarkerIgnoringBase
> 使用低于Maven 3.3.3的版本，我用的是Maven 3.0.5

2. node-gyp rebuilt卡住
原因是node-gyp rebuilt 会去下载源码rebuilt生成头文件，而国外的npm源非常慢，所以可以手动修改
执行：
```
npm install node-gyp 
node-gyp install --dist-url http://npm.taobao.org/mirrors/node
```
或者直接修改：

```
# 获得 nodejs 版本号
NODE_VERSION=`node -v | cut -d'v' -f 2`
# 下载源码包
wget http://npm.taobao.org/mirrors/node/v$NODE_VERSION/node-v$NODE_VERSION.tar.gz
# 删除现有内容不完整的目录
rm -rf ~/.node-gyp
mkdir ~/.node-gyp
# 解压缩并重命名到正确格式
tar zxf node-v$NODE_VERSION.tar.gz -C ~/.node-gyp
mv ~/.node-gyp/node-v$NODE_VERSION ~/.node-gyp/$NODE_VERSION
# 创建一个标记文件
printf "9\n">~/.node-gyp/$NODE_VERSION/installVersion
```

