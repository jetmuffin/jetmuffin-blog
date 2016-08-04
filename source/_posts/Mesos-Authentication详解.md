title: Mesos Authentication详解
date: 2015-12-28 17:01:24
tags: [mesos,authentication]
categories: 云计算
---
Authentication模块是Mesos中较为重要的一部分，涉及了整个系统的安全性。Mesos 0.15.0引入了framework authentication，0.19.0引入了slave authentication。Mesos Authentication只允许信任的实体可以和Mesos集群进行交互。

<!-- more -->

Mesos中的Authentication模块常用使用方法有三种：

1. 要求framework必须被授权才能向master注册
2. 要求slaves必须被授权才能获得resources offer
3. 限制`/teardown`端的访问

## 工作原理

Mesos的Authentication模块使用了`Cyrus SASL Library`实现验证。SASL提供了多种验证机制，包括（`ANOYMOUS`,`PLAIN`,'CRAM-MD5`,`GSSAPI`等）。

现在Mesos默认支持`CRAM-MD5`进行验证，单用户可以拓展自己的`authentication modules`（详见之后的文章[mesos modules](#)）。`CRAM-MD5`使用`principal`和`secret`对进行验证，其中`principal`代表framework的身份。**注意，这里的framework并不是执行framework的user而是framework自身**

## 配置

在启动mesos节点时可以通过配置项来选择是否启用authentication模块，对哪些部分启用authentication。

### Masters

* -authenticate/-no-authenticate 前者只允许授权的framework接入集群，后者还允许未授权的framework接入
* -authenticate_slaves/-no-authenticate_slaves 前者只允许授权的slave介入集群，后者还允许未授权的framework接入
* -authenticators 指定选用的`authenticator module`默认是`crammd5`，用户可以通过`-modules`选项进行拓展
* -credentials 指定证书路径（可能因`authenticator module`不同而不一定使用该选项）

### Slaves

* -authenticatee 指定slave使用哪个`authenticator module`和master的`authenticators`进行授权。默认是`crammd5`
* -credential 指定证书路径

## 例子

这里使用默认的`CRAM-MD5`进行验证。

**1.创建credentials文件**

首先为master创建credentials文件，其中的内容像如下所示：

```
principal1 secret1
principal2 secret2
```

**2. 启动master**

然后启动master,并使用我们刚刚生成的credentials文件
```
/bin/mesos-master.sh --ip=127.0.0.1 --work_dir=/var/lib/mesos --authenticate --authenticate_slaves --credentials=~/credentials
```

**3.创建slave的credentials**

像步骤1一样创建slave的credentials

```
principal1 secret1
```

**4.启动slave**

启动slave并进行验证：

```
./bin/mesos-slave.sh --master=127.0.0.1:5050 --credential=~/slave_credential
```

**6.framework验证**

不同的framework的验证方法可能不同，可以写入`scheduler driver`的构造方法中，也可以在启动框架时加入，如下：

```
MESOS_AUTHENTICATE=true DEFAULT_PRINCIPAL=principal2 DEFAULT_SECRET=secret2 ./src/test-framework --master=127.0.0.1:5050

```

结果：

使用credentials验证成功的slave:

![authenticate_success](http://7xpl2y.com1.z0.glb.clouddn.com/QQ20151228-0.png)

未使用credentials验证的slave:

![authenticate_failed](http://7xpl2y.com1.z0.glb.clouddn.com/QQ20151228-1.png)

可以看到未使用的slave无法进行注册




