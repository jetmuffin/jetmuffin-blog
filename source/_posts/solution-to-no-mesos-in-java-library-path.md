title: 运行Mesos Framework报no mesos in java.library.path解决方法
date: 2016-01-05 15:08:33
tags: [mesos,bugfix]
categories: 设计开发
---

在运行自己用scala编写的Framework时遇到了一个巨大的问题，即
<!-- more -->
在执行

```
java -cp xxxx.jar -Djava.library.path=libs xxxx.xxxx.xxx
```

之后报错：

```
Exception in thread "main" java.lang.UnsatisfiedLinkError: no mesos in java.library.path
    at java.lang.ClassLoader.loadLibrary(ClassLoader.java:1758)
    at java.lang.Runtime.loadLibrary0(Runtime.java:823)
    at java.lang.System.loadLibrary(System.java:1045)
    at org.apache.mesos.MesosNativeLibrary.load(MesosNativeLibrary.java:46)
```

尝试了把build之后的`mesos-0.25.0.jar`和`protobuf-2.5.0.jar`加入，依然不生效，查询了`stackoverflow`后发现是`MESOS_NATIVE_LIBRARY`环境变量未加入。

因此只要执行

```
 # For Linux
$ export MESOS_NATIVE_LIBRARY='/usr/local/lib/libmesos.so'

 # For OSX
$ export MESOS_NATIVE_LIBRARY='/usr/local/lib/libmesos.dylib'
```
就行了。

为了方便，可以把这个语句加到.bashrc中
