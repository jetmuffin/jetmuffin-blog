title: Golang 基础学习（一）
date: 2016-09-03 20:09:56
tags: [golang,hello world]
categories: 编程语言
thumbnail: http://cdn.jetmuffin.com/posts/golang.png
---

## 目的

最初接触 go 是在 15 年，最开始对 go 的理解是写多线程的程序非常方便，于是学习了 go 的基本语法，并尝试写了一些 go 的简单应用，包括我的本科毕业设计也是使用 go 写的（[apt-mesos](https://github.com/icsnju/apt-mesos)）。但是对 go 也是停留在最基本的语法认识，有很多特性并未了解。所以想要从头再屡一遍 go 的所有内容。对每一块儿内容，以重学的角度深入学习。

<!-- more -->

## 教程与代码
学习的过程将使用教程（[https://gobyexample.com/](https://gobyexample.com/)），它对 go 的每块儿内容提供了 demo，可以更易于理解。教程中的例程，将对其进行理解和实现，然后将代码整理在仓库 [[JetMuffin/golearning](https://github.com/JetMuffin/golearning.git)]中。

## Hello World

任何语言的入门，都需要编写一个 Hello World 程序。

```go
package main

import "fmt"

func main() {
    fmt.Println("hello world")
}
```

这是一段使用 `fmt` 进行输出 `hello world` 字符的代码，从语言的风格上看，go 使用 `package` 作为关键字进行包管理，这和 java 很像；而导入包的部分则又像 python；而主函数部分也比较独特。庆幸的是，go 中也不需要添加分号。

go 和 python，ruby 等不同，它是解释型语言，因此在每次运行前需要进行编译，然后才可以进行执行，或者直接进行编译执行。

```bash
$ go run helloworld.go
hello world

$ go build helloworld.go
$ ls
helloworld      helloworld.go

$ ./helloworld
hello world
```

这里需要知道的一点是，编译好后生成了 binary 文件，可以在同平台下直接进行运行，但并无法做到 runing everywhere，因为可能需要涉及到交叉编译的问题。

