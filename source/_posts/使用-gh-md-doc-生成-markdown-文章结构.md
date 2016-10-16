title: 使用 gh-md-doc 生成 markdown 文章结构
date: 2016-09-13 20:23:11
tags: [markdown, gh-md-doc]
categories: 设计开发
---

本篇技巧来自于我在 [iQiyi](http://www.iqiyi.com) 的云平台部门实习时的内部分享，主要用于文档编写。
在开发开源项目或进行团队协作时，总要写一些文档，并把代码托管在 git 上，对于长篇文档希望能够生成文档的结构，这里就用到了一个非常好的插件 [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)。

<!-- more -->

## 生成 markdown table of contents
---

常常项目开发的文档都是用 markdown 格式编写，并且以代码的形式托管在 Git 上，
同时，那么就可以使用 [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)
来生成文档目录。

但是，gh-md-toc 会直接将结果打印到 stout，而我们想要的最好的效果是直接将其添加到文档头部，
而不是从 stdout 再复制粘贴，然后还要格式化空行。

以本文档为例，gh-md-toc 生成的 table of contents 如下：

```
Table of Contents
=================

    * [生成 markdown table of contents](#生成-markdown-table-of-contents)
    * [使用 gh-md-toc 的奇技淫巧](#使用-gh-md-toc-的奇技淫巧)

Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc)
```

显然，我们需要将这段文字添加到文件的头部，并且**删除第一行空行，在最后一行添加一行空行**。

## 使用 gh-md-toc 的奇技淫巧

OK，Let's Rock!

先看一条命令：

```
$ gh-md-toc file.md | (tail -n +2 && echo "" && cat file.md) | sponge file.md
```

你会惊讶的发现，这条命令就完成了所有事，为 file.md 生成了 table of
contents，并且自动添加在了 file.md 的头部，并且完美的处理了空行。

这里使用了一个好工具 `sponge`，想想为什么不能直接重定向到 file.md 呢？

但是，追求极致的程序员不会止步，因为上面的命令中 file.md 这个待处理的文件出现了
3 次，所以也挺烦人的，对吧，即使使用命令替换也来得不如一次直接。

OK，Let's Rock Again!

添加一个函数，重定义 gh-md-toc，添加下面的函数定义到你的 shell rc，例如：

  - bash，~/.bashrc
  - zsh，~/.zshrc

```bash
function gh-md-toc() {
  if [ -z "$1" ]; then
      echo "gh-md-toc <markdown file>"
      return 1
  fi
  /home/chengwei/Github/github-markdown-toc/gh-md-toc $1 | (tail -n +2 && echo "" && cat $1) | sponge $1
}
```

将上面的 `/home/chengwei/Github/github-markdown-toc/gh-md-toc` 替换成 gh-md-doc
在你的系统上的路径。

**注意：如果你把 gh-md-doc 放在了 $PATH 中，必须用绝对路径调用，否则就无限递归了 :-)**

最后，source shell rc 配置文件，生效

```bash
$ source ~/.bashrc
OR
$ source ~/.zshrc
```

现在，要为某个 markdown 文件添加 table of contents 只需要执行：

```bash
$ gh-md-toc file.md
```

