title: Debian下编译shadowsocks-qt5
date: 2016-10-18 13:53:08
tags: [shadowsocks,debian]
categories: 设计开发
---

平时开发用的是 debian 的 linux 发行版，而 shadowsocks-qt5 没有提供 debian 的 release 版本，于是得自己动手编译，编译的时候会遇到许多坑，这里记录一下。

<!-- more -->

### libQtshadowsocks

首先需要编译 shadowsocks 的 lib 库 `libQtShadowsocks`，从 [github/libQtshadowsocks](https://github.com/shadowsocks/libQtShadowsocks/releases) 上找到稳定版本，直接下载源码的 .tar.gz 文件到本地，解压，并进入文件夹。

wiki 上对编译的依赖描述如下：

> Qt >= 5.2 (qt5-qtbase-devel and qt5-qttools in Fedora, qtbase5-dev and qt5-qmake in Debian/Ubuntu)
Botan >= 1.10 (botan-devel in Fedora, libbotan1.10-dev in Debian/Ubuntu)
A C++ Compiler that supports C++11 features (i.e. GCC >= 4.7) 

但是只安装这些依赖，在 debian 下会出现 `botan-1.10 development package not found` 的错误，但是检查后发现 `libbotan1.10-dev` 包已经安装了。

后来发现是有其他的依赖没满足，所以干脆一次性把所有依赖全部装了，就不会出现问题了。

```
sudo apt-get install qt5-qmake qtbase5-dev libqrencode-dev libappindicator-dev libzbar-dev libbotan1.10-dev
```

然后进入进行编译，编译时可以选择 `INSTALL_PREFIX`，它默认是 `/usr` 目录，如果自行更改会有另外的问题，下面会提到。

```
qmake
make && make install
```

### shadowsocks-qt5

编译安装完 `libQtShadowsocks` 后就可以编译 `shadowsocks-qt5` 了。从 [https://github.com/shadowsocks/shadowsocks-qt5](https://github.com/shadowsocks/shadowsocks-qt5) 上 clone ss-qt5 的源码，然后进入目录编译：

```
git clone https://github.com/shadowsocks/shadowsocks-qt5.git
cd shadowsocks-qt5
qmake
make && make install
```

同样这里的 `qmake` 可以指定 `INSTALL_PREFIX`, 默认为 `/usr`。安装后 ss-qt5 会在 `/usr/bin` 文件夹中，命令行直接运行 `ss-qt5` 或者在图形界面中直接搜索就可以运行了。

### by the way

如果在运行时仍然碰到 `libQtShadowsocks.so.1 not found` 这个问题，那么原因就可能是上面所提到的编译 `libQtShadowsocks` 的问题了。默认指定的 `INSTALL_PREFIX` 为 `/usr`，即安装到 `/usr/lib` 中，而这个路径是自动被加到系统的链接库里的。

而如果自行指定了 `INSTALL_PREFIX`，例如我选择了 `/usr/local`，那么这个 so 文件会被安装到 `/usr/local/lib`，而这个目录不是默认被加到系统链接库里的，那么需要手动加进去，解决方法如下：

```
echo "/usr/local/lib" >> "/etc/ld.so.conf"
/sbin/ldconfig
```

然后再运行 ss-qt5 就没有问题了。
