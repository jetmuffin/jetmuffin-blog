title: Golang 基础学习（二）类型
date: 2016-09-04 10:25:44
tags: [golang]
categories: 编程语言
toc: true
---

## 类型

go 语言中有许多的类型，包括字符串、整形、浮点型、布尔型等等，这和一般的语言相同。这些类型之间可以进行基本的运算，这和 C, Java 等语言也是相同的。

<!-- more -->

```go
package main

import (
    "fmt"
    "math/cmplx"
)

var (
    flag   bool       = false
    MaxInt uint64     = 1<<64 - 1
    z      complex128 = cmplx.Sqrt(-5 + 12i)
)

func main() {

    fmt.Println("go" + "lang")

    fmt.Println("1+1 = ", 1+1)
    fmt.Println("7.0/3.0 =", 7.0/3.0)

    fmt.Println(true && false)
    fmt.Println(true || false)
    fmt.Println(!true)

    const f = "%T(%v)\n"
    fmt.Printf(f, flag, flag)
    fmt.Printf(f, MaxInt, MaxInt)
    fmt.Printf(f, z, z)
}
```

### 基础类型

go 语言中的基础数据类型包括以下几种：

- int，Rune（Rune 是 int 的别名，代表一个 Unicode 码）
- int8，int16，int32，int64
- byte，uint8，uint16，uint32，uint64（byte 是 uint8 的别名）
- float32，float64
- bool
- string
- complex128，complex64（复数）

源码中 `runtime.h` 对几种简单的基本类型先做了定义，然后用它们对其他的类型进行定义。

```cpp
typedef signed char             int8;
typedef unsigned char           uint8;
typedef signed short            int16;
typedef unsigned short          uint16;
typedef signed int              int32;
typedef unsigned int            uint32;
typedef signed long long int    int64;
typedef float                   float32;
typedef double                  float64;

typedef uint32                  uintptr
typedef int32                   intptr
typedef int32                   intgo
typedef uint32                  uintgo

typedef uint8                   bool;
typedef uint8                   byte;
```

从中可以看出 go 中的每个类型在 c 中对应的类型和占用字节大小。另外其中还定义了 `intptr` 和 `intgo`，其中 `intptr` 用于指针运算，`intgo` 相当于在源码中给 `int` 起的一个别名。

### string

string 类型事实上是一个结构体：

```
struct String {
    byte*   str;
    intgo   len;
}
```

这里对于每个 string 类型，都确定了它的长度和字符数组，也就是说 golang 的字符串初始化话底层的结构就会被初始化。

## 变量

go 中使用 `var` 作为关键字来进行变量声明。但和 javascript 里不同，go 中的变量是强类型。声明的方式有多种，包括：

- `var a int`（仅声明不初始化）
- `var a = 10`（声明同时初始化，但不指定类型，由编译器自动推导类型）
- `var a int = 10`（声明、指定类型以及初始化）
- `a := 10`（省略关键字，声明同时初始化，由编译器推导类型）

**Note:** 这里需要注意的是，`:=`是用于声明及赋值，而不是赋值，因此仅在变量定义时使用，开始写 go 的时候常常将 `:=` 用于赋值导致出错。

```go
package main

import "fmt"

func main() {
    var a string = "initial"
    fmt.Println(a)

    var b, c int = 1, 2
    fmt.Println(b, c)

    var d = true
    fmt.Println(d)

    var e int
    fmt.Println(e)

    f := "short"
    fmt.Println(f)
}
```

### 类型推导

对变量声明语句，编译器会对其进行自动的类型推导，可以用 `reflect` 包来查看变量的类型。

```go
package main

import (
    "fmt"
    "reflect"
)

func main() {
    var a int = 10
    var b byte = 10

    c := 10.0
    d := "10"

    fmt.Printf("a type:%s\n", reflect.TypeOf(a))
    fmt.Printf("b type:%s\n", reflect.TypeOf(b))
    fmt.Printf("c type:%s\n", reflect.TypeOf(c))
    fmt.Printf("d type:%s\n", reflect.TypeOf(d))
}
```

结果如下：

```
a type:int
b type:uint8
c type:float64
d type:string
```

### 类型转换

正常情况下，将类型 V 转化为 T 需要显式声明。

```
a := 5 // typeof(a) is int32
b := int64(10) // typeof(b) is int64
```

若没有显式声明还需要进行类型转换 V -> T，则需要满足 `assignability`，根据文档[assignability](https://golang.org/ref/spec#Assignability)，可能的条件如下：

- T 和 V 的类型相同
- T 和 V 的基础类型相同，并且其中至少一个并未显式指定类型
- T 是一个 `interface` 类型，并且 V 实现了 T (这在后面会提到)
- V 是一个双向的 `channel`，而 T 是一个 `channel`，同时 V 和 T 的元素类型相同
- V 是 nil 同时 T 是 `slice`, `function`, `map`, `channel`, `interface`, `pointer` 中的一种
- V 是未声明类型的常量

在这些条件下，不需要显式声明，直接可以将 T 的值赋予 V。

## 常量

go 中的常量和 c 相同，需要在编译时确定值，并在编译时被创建。常量需要满足以下几个要求：

- 类型必须是数值、字符串和布尔值之一
- 可以使用表达式，但表达式必须在编译时可以计算的
- 声明常量同时必须指定值（初始化），并且不能再修改值

常量的关键字和 cpp 相同，用 `const` 进行声明，声明时可以显式指定数据类型，也可以不指定。多个常量可以同时进行批量声明，常见的使用方法如下：

```go
package main

import (
    "fmt"
    "math"
)

const (
    s string = "constant"
    one, two = 1, 2
)

func main() {
    fmt.Println(s)
    fmt.Println(one)
    fmt.Println(two)

    const n = 500000000
    const d = 3e20 / n
    fmt.Println(d)

    fmt.Println(int64(d))

    fmt.Println(math.Sin(n))
}
```

### iota

iota 在 go 中用于常量计数。iota 在 const 关键字出现时置 0，并且 const 内每增加一行讲使 itoa 计数一次，举个例子应该就很清楚了。

```go
const a = iota // a = 0
const b = iota // b = 0
const (
    c = iota   // c = 0
    d = iota   // d = 1
    e = iota   // e = 2
)
const (
    f, g = iota, 1 << iota // f = 0, g = 1
    h, i = iota, 1 << iota // h = 1, i = 2
)

## 参考资料

- [Golang Document](https://golang.org/ref/spec#Constant)
- [老虞学GoLang笔记](http://www.cnblogs.com/howDo/archive/2013/04/15/GoLang-Constant.html)
- [golang的类型转换的坑和分析](http://www.tuicool.com/articles/MJZvi27)