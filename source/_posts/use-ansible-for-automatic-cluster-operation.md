title: 使用Ansible进行服务器自动化运维
date: 2017-04-05 19:04:04
tags: [ansible, centos]
categories: 系统运维
---

> ansible是新出现的自动化运维工具，基于Python开发，集合了众多运维工具（puppet、cfengine、chef、func、fabric）的优点，实现了批量系统配置、批量程序部署、批量运行命令等功能。

<!--more-->

## 测试环境

Virtualbox && Vagrant，安装 CentOS 7。用 Vagrant 自动分配 CPU、内存以及 IP。共两台虚拟机，一台为 master，一台为 slave，以模拟集群下环境。

- IP: 192.168.32.10(master) 192.168.32.11(slave)
- 子网掩码：255.255.255.0
- 用户：root

## Ansible 脚本使用流程

### 安装 Ansible

Ansible 可以直接通过 yum 的 epel 源进行安装。

```bash
yum -y install epel-release
yum -y install ansible
```

### 配置 ssh 无密码登陆

Ansible 在集群的机器之间所有的操作都是基于 ssh 进行的，因此需要配置每台机器上 master 的无密码登陆。这里我们只要配置 slave 这台机器即可。

```bash
ssh-keygen -t rsa -P ""
ssh-copy-id slave
```

配置完 ssh-key 后尝试从 master 登录 slave，若不需密码则配置成功。

### Ansible 脚本使用

从 gitlab 上 clone Ansible 运维的 playbooks 至 master，playbooks 的目录层级如下：

```bash
cd ics-cluster/ansible
tree
├── bin
│   └── setup.sh
├── firewall.yml
├── group_vars
│   └── all
├── hosts
├── keys.yml
├── roles
│   ├── authorized_keys
│   ├── autofs
│   ├── common
│   ├── firewalld
│   ├── nfs
│   ├── nis
│   ├── ntp
│   └── user
├── setup.retry
├── setup.yml
└── user.yml
```

其中根目录下的 `yml` 文件为指定功能的 playbook，运行方法为：

```bash
ansible-playbook -i host xxx.yml
```

若需要对过程进行 debug，在命令后加上 `-vvv` 参数即可。

### Playbook 编写

编写 playbook 需要按照在单机上配置软件的过程，大体包括三个部分：安装软件、配置软件和启动服务。因此 `package`，`template` 和 `service` 是经常使用的三个模块。

根据具体需要部署的软件的部署流程，依次在 playbook 中写好每个阶段的任务。其中可能还会涉及到选择分支等高级用法，具体详询 [Ansible Playbook Documents](http://docs.ansible.com/ansible/playbooks.html)。

## 具体问题解决

在配置集群环境以及编写 ansible playbooks 的过程中，会遇到非常多的坑，这里记录了一些可能会遇到的问题。

#### 1. ansible 在某个节点 setup 阶段卡住

Setup 阶段 ansible 主要收集机器节点的一些信息，而在这个阶段卡住原因可能出在文件系统（fs）和挂载点（mounts）上。因此登录到这个节点上使用 `df` 或者 `mount` 去人为检查文件系统是否出现问题。

#### 2. 在 / 目录下 df 或 ls 卡住

出现这种状况极有可能的原因是挂载点的问题。检查 `/etc/fstab` 中所有挂载点是否正常。如果在 `/etc/fstab` 中配置了自动挂载 nfs，那么如果 nfs-server 挂掉了，就会导致 nfs-client 上的 `df` 或 `ls` 卡住。

当然在这种状况下使用 `umount /exports` 也会卡住，需要使用 `umount -l /exports` 来解除挂载。

#### 3. ypbind 服务无法启动

配置 NIS 时，在 client 上启动 `ypbind` 服务时会出现启动超时的问题，主要的原因是 client 无法和 server 端通信。以下是两个排查的方向：

1. 检查 server 端的防火墙配置（iptables 或 firewalld），打开 `ypserv` 和 `yppasswdd` 的端口
2. server 端在初次启动 `ypserv` 后需要对 yp 进行初始化，运行 `ypinit -m`

## 参考文档

- [Ansible Documentation](docs.ansible.com)
- [鸟哥的Linux 私房菜-- NFS 服务器
](http://cn.linux.vbird.org/linux_server/0330nfs.php)
- [Configure NIS Server](https://www.server-world.info/en/note?os=CentOS_7&p=nis)
- [Configure NIS Client](https://www.server-world.info/en/note?os=CentOS_7&p=nis&f=2)