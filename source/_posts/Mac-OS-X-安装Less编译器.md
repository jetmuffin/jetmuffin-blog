title: Mac OS X 安装Less编译器
date: 2015-11-18 19:46:37
tags: [less]
categories: 前端开发
---
Less是动态的CSS预处理语言，包含了很多动态语言的特性，之前在ubuntu下装过lessc来编译Less,换了Mac后发现找不到方法安装了，事实上用nodejs来安装，这里记录一下。

<!-- more -->

在原Ubuntu下安装lessc很简单，直接执行`sudo apt-get install node-less`就行了

然而在Mac OS X下，`brew install node-less`找不到相应的包，需要用`nodejs`来安装：

1. 安装`nodejs`和`npm`
2. 执行`npm install -g less`
3. 编译something `lessc style.less`

同时我们可以在WebStorm中使用`file watcher`来监视文件的修改，直接生成css

1. 找到`lessc`的位置，`which lessc`,一般lessc都在`/usr/local/bin/lessc`
2. 在WebStorm中的"Preferences"里找到"File Wathcers"选项，在其中添加Less的watcher，其中的Program填入`/usr/local/bin/lessc`就行了

效果如图:

![filewatcher](/uploads/images/2015/11/18/QQ20151118-0@2x.png)
