title: "How to fix '/bin/rm: Argument list too long"
date: 2016-11-13 16:09:57
tags: [linux]
categories: 系统运维
---
做运维时发现系统的磁盘满了，检查发现磁盘的 `/tmp` 目录下装满了东西，于是很简单的想用 `rm -rf /tmp/*` 来删除，结果发现报错：`bash: /bin/rm: Argument list too long`。找了找方法解决它。
<!--more-->

当执行命令时：

```
/bin/rm -rf ./*
```

得到了错误提示：

```
bash: /bin/rm: Argument list too long
```

查看发现，`rm` 命令也有参数长度的限制，所以需要用管道将命令分批次输入到 `rm` 中去。

因此 `xargs` 就派上用场了

```
find . -name "_MEI*" | xargs /bin/rm -rf
```

或者

```
find . -name "_MEI*" -print0 | xargs /bin/rm -rf
```

或者更直接的用 `find` 的 `-delete` 参数

```
find . -name "_MEI*" -delete
```

即可解决。