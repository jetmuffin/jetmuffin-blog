title: 使用Mesos和Marathon搭建Docker集群
date: 2015-11-09 17:27:14
tags: [mesos,marathon,docker]
categories: 基础架构
---

利用Mesos，Marathon对Docker集群进行管理，保证高容错性，高拓展性。

<!-- more -->

## 部署

### 1.创建Master节点虚拟机
执行命令

```
$: vagrant init
```

这时候会在当前文件夹下创建文件`Vagrantfile`，该文件是`ruby`语法的虚拟机配置文件，然后我们用vim修改该文件：

```
$: vim Vagrantfile
```

将内容修改如下：

```ruby
# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "bento/centos-7.1"
  config.ssh.insert_key = false
  config.vm.define "node1" do |node1|
      node1.vm.network "private_network", ip: "192.168.33.10"
      node1.vm.hostname = "node1"
  end

  config.vm.define "node2" do |node2|
      node2.vm.network "private_network", ip: "192.168.33.11"
      node2.vm.hostname = "node2"
 end
end
```

该配置会生成两个使用系统centos-7.1的节点，分别分配IP为`192.168.33.10`和`192.168.33.11`，作为我们的`Master`和`Slave`。

然后我们启动第一个节点`node1`:

```
$: vagrant up node1
Bringing machine 'node1' up with 'virtualbox' provider...
==> node1: Checking if box 'bento/centos-7.1' is up to date...
==> node1: Clearing any previously set forwarded ports...
==> node1: Clearing any previously set network interfaces...
==> node1: Preparing network interfaces based on configuration...
    node1: Adapter 1: nat
    node1: Adapter 2: hostonly
==> node1: Forwarding ports...
    node1: 22 => 2222 (adapter 1)
==> node1: Booting VM...
==> node1: Waiting for machine to boot. This may take a few minutes...
    node1: SSH address: 127.0.0.1:2222
    node1: SSH username: vagrant
    node1: SSH auth method: private key
    node1: Warning: Connection timeout. Retrying...
    node1: Warning: Remote connection disconnect. Retrying...
==> node1: Machine booted and ready!
==> node1: Checking for guest additions in VM...
==> node1: Setting hostname...
==> node1: Configuring and enabling network interfaces...
==> node1: Mounting shared folders...
    node1: /vagrant => /Users/jeff/workspace/mesos/cluster
==> node1: Machine already provisioned. Run `vagrant provision` or use the `--provision`
==> node1: flag to force provisioning. Provisioners marked to run always will still run.
```

这样第一个节点就成功启动了，然后我们通过`ssh`连接到`node1`进行操作

```
$:vagrant ssh node1
[vagrant@node1 ~]$
```

这样就成功创建了第一个节点。

### 2.下载启动Mesos和Marathon

在下载`Mesos`和`Marathon`之前，需要先把**hosts文件修改一下**，把`127.0.0.1`对应的解析中的`node1`删除，并在最后加上`node1`和`node2`的IP，修改后的`hosts`可见cat后的输出

```
[vagrant@node1 ~]$ sudo vi /etc/hosts
[vagrant@node1 ~]$ cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6

192.168.33.10 node1
192.168.33.11 node2
```

然后下载mesos的仓库，再安装mesos和marathon

```
[vagrant@node1 ~]$ sudo rpm -Uvh http://repos.mesosphere.com/el/7/noarch/RPMS/mesosphere-el-repo-7-1.noarch.rpm
[vagrant@node1 ~]$ sudo yum install mesos marathon
```

### 3.下载配置Zookeeper

下载zookeeper仓库并安装Zookeeper

```
[vagrant@node1 ~]$ sudo rpm -Uvh http://archive.cloudera.com/cdh4/one-click-install/redhat/6/x86_64/cloudera-cdh-4-0.x86_64.rpm
[vagrant@node1 ~]$ sudo yum -y install zookeeper zookeeper-server
```

初始化并启动Zookeeper

```
[vagrant@node1 ~]$ sudo -u zookeeper zookeeper-server-initialize --myid=1
Using myid of 1
[vagrant@node1 ~]$ sudo service zookeeper-server start
JMX enabled by default
Using config: /etc/zookeeper/conf/zoo.cfg
Starting zookeeper ... STARTED
```

### 4.下载安装Docker

```
[vagrant@node1 ~]$ sudo yum install -y docker
[vagrant@node1 ~]$ sudo chkconfig docker on
[vagrant@node1 ~]$ sudo service docker start
```

然后告诉mesos，允许使用Docker作为Container

```
[vagrant@node1 ~]$ echo 'docker,mesos' | sudo tee /etc/mesos-slave/containerizers
[vagrant@node1 ~]$ sudo service mesos-slave restart
```

### 5.启动Mesos和Marathon

启动Mesos

```
[vagrant@node1 ~]$ sudo service mesos-master start
[vagrant@node1 ~]$ sudo service mesos-slave start
```

这时可以从外部访问已经启动的Mesos的GUI界面了，访问地址为`192.168.33.10:5050`

![start-mesos](/uploads/images/2015/11/09/1.png)

启动Marathon

```
[vagrant@node1 ~]$ sudo service marathon start
```

同样可以从GUI访问Marathon了，访问地址为`192.168.33.10:8080`

![start-marathon](/uploads/images/2015/11/09/2.png)

至此，Master节点的配置就已经结束了

### 6.创建配置Slave节点

首先启动slave节点并用ssh登录

```
$: vagrant up node2
$: vagrant ssh node2
[vagrant@node2 ~]$
```

然后修改`/etc/hosts`并且下载Mesos，具体过程同[步骤4](#)。

```
[vagrant@node2 ~]$ sudo vi /etc/hosts
[vagrant@node2 ~]$ cat /etc/hosts
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6

192.168.33.10 node1
192.168.33.11 node2
[vagrant@node2 ~]$ sudo rpm -Uvh http://repos.mesosphere.com/el/7/noarch/RPMS/mesosphere-el-repo-7-1.noarch.rpm
[vagrant@node2 ~]$ sudo yum install mesos
```

编辑node2的`/etc/mesos/zk`文件，使得它指向node1。

```
[vagrant@node2 ~]$ sudo vi /etc/mesos/zk
[vagrant@node2 ~]$ cat /etc/mesos/zk
zk://192.168.33.10:2181/mesos
```

启动Mesos（作为slave），并且关闭`mesos-master`服务的自动启动

```
[vagrant@node2 ~]$  sudo service mesos-slave start
[vagrant@node2 ~]$ sudo chkconfig mesos-slave on
[vagrant@node2 ~]$  sudo chkconfig mesos-master off
```

安装Docker,并允许使用Docker作为容器

```
[vagrant@node2 ~]$ sudo yum install -y docker
[vagrant@node2 ~]$ sudo chkconfig docker on
[vagrant@node2 ~]$ sudo service docker start
[vagrant@node2 ~]$ echo 'docker,mesos' | sudo tee /etc/mesos-slave/containerizers
[vagrant@node2 ~]$ sudo service mesos-slave restart
```

这样，slave节点的配置也结束了

### 6.测试

我们打开Mesos的GUI界面，查看slave，可以看到现在已经有2个slave节点了，如下图。

![check-slaves](/uploads/images/2015/11/09/3.png)

然后我们创建一个简单的webapp，使用`python -m SimpleHTTPServer`

![create-webapp](/uploads/images/2015/11/09/4.png)

然后将webapp应用scale到2个实例

![scale-instance](/uploads/images/2015/11/09/5.png)

可以看到两个实例分别在两个不同的slave中运行了

![result](/uploads/images/2015/11/09/6.png)

```
//Todo 后面会加上部署Docker的实例测试
```

## 打包虚拟机

为了之后能够快速的部署，我们可以将`node1`和`node2`分别打包成`mesos-master`和`mesos-slave`，以便于增加节点。

退出ssh，首先将node1打包为`mesos-master`，在宿主机上执行：

```
$: vagrant package node1
$: vagrant box add mesos-master package.box
$: vagrant destroy node1
```
同样对node2我们也将其打包为`mesos-slave`

```
$: vagrant package node2
$: vagrant box add mesos-slave package.box
$: vagrant destroy node2
```

然后修改`Vagrantfile`，将其修改为：

```
# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "bento/centos-7.1"
  config.ssh.insert_key = false
  config.vm.define "node1" do |node1|
      node1.vm.network "private_network", ip: "192.168.33.10"
      node1.vm.hostname = "node1"
      node1.vm.box = "mesos-master"
  end

  config.vm.define "node2" do |node2|
      node2.vm.network "private_network", ip: "192.168.33.11"
      node2.vm.hostname = "node2"
 end
end
```

这样就完成了虚拟机的打包，然后分别运行：

```
$: vagrant up node1
$: vagrant up node2
```

就可以运行了，修改集群的slave数量，则可以直接修改`Vagrantfile`即可！