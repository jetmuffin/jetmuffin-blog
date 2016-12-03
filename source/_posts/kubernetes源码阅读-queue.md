title: kubernetes源码阅读 - queue
date: 2016-12-03 16:04:00
tags: [kubernetes, queue]
categories: 基础架构
thumbnail: http://ohlxaxn1q.bkt.clouddn.com/kubernetes.jpg
---
上篇文章中对 k8s 的 scheduler 部分进行了简单的源码阅读和分析，现在接着上次的位置继续往下阅读。

<!--more-->

## 引入

在 `plugin/pkg/scheduler/factory/factory.go` 的代码中，scheduler 在创建时引用了一个 `getNextPod` 方法，获取下一个待调度的 pod。

```go
// plugin/pkg/scheduler/factory/factory.go

func (f *ConfigFactory) getNextPod() *v1.Pod {
    for {
        pod := cache.Pop(f.PodQueue).(*v1.Pod) // 获取下一个待调度的 Pod
        if f.responsibleForPod(pod) {
            glog.V(4).Infof("About to try and schedule pod %v", pod.Name)
            return pod
        }
    }
}
```

当时没有继续下去，这些 pod 在 k8s 的调度队列里是如何管理的，这也是这篇文章想从源码里学到的。

## Queue

和上篇文章一样，由于 k8s 的代码量非常大，那么我们还是一点一点追溯到代码的源头。从 `cache.Pop()` 方法往上头寻找，IDE 会把你带到 `pkg/client/cache/fifo.go` 这个文件。从名字很明显看出这是一个先进先出的队列，那么我们就需要看看 k8s 在队列上是如何进行设计的。

首先是 `Queue` 的接口：

```go
// pkg/client/cache/fifo.go

type Queue interface {
    Store // 实际存储的后端

    // Pop blocks until it has something to process.
    // It returns the object that was process and the result of processing.
    // The PopProcessFunc may return an ErrRequeue{...} to indicate the item
    // should be requeued before releasing the lock on the queue.
    Pop(PopProcessFunc) (interface{}, error) // pop 的抽象方法

    // AddIfNotPresent adds a value previously
    // returned by Pop back into the queue as long
    // as nothing else (presumably more recent)
    // has since been added.
    AddIfNotPresent(interface{}) error // 如同注释所说。。加入队列如果它不在队列中

    // Return true if the first batch of items has been popped
    HasSynced() bool
}
```

这个接口实际上是对所有类型的队列进行抽象，`Pop()` 方法是它们共有的方法，FIFO 是一种队列的实现方式，队列还可能有其他的实现方式，诸如 FILO 等。

### FIFO Queue

k8s 中默认使用的是 FIFO 队列：

```go
// pkg/client/cache/fifo.go

type FIFO struct {
    lock sync.RWMutex // 读写锁
    cond sync.Cond // 读写锁基础上的消息通知对象
    // We depend on the property that items in the set are in the queue and vice versa.
    items map[string]interface{} // 存储对象的内容
    queue []string // 存储对象的 id

    // populated is true if the first batch of items inserted by Replace() has been populated
    // or Delete/Add/Update was called first.
    populated bool
    // initialPopulationCount is the number of items inserted by the first call of Replace()
    initialPopulationCount int

    // keyFunc is used to make the key used for queued item insertion and retrieval, and
    // should be deterministic.
    keyFunc KeyFunc // 用于获取item的key的function
}
```

FIFO 队列中有两个重要的数据结构：

- `items map[string]interface{}` 用于 map 的 键值对形式实际存 items，便于 items 的增删改查
- `queue []string` 用于维护每个 items 进入 FIFO 队列的先后顺序，只存 id

这里和其他用 chanel 实现的 FIFO 队列稍有不同，用一个 map 和一个 list 就成功的实现了可以增删改查的先进先出队列。

而这个先进先出的 FIFO 队列共有以下操作：

- `Add()` （添加一个 item）
- `AddIfNotPresent()` （如果 item 不在队列中，那么就把它加进队列里）
- `Update()` （更新指定 item）
- `Delete()` （删除制定 item）
- `List()` （列举所有的 item）
- `ListKeys()` （列举所有的 key）
- `Get()` (按对象获取 item）
- `GetByKey()` （按 key 获取 item）
- **`Pop()`**
- `Replace()` （替换整个队列的内容）
- **`Resync()`**

前几个方法比较常规，就是一些增删改查的方法。不过需要注意的是，这个 FIFO 在 Delete 时只是删去它在 map 里的对象，不会删去 queue 里的 id，而是在 Pop 时检查一下这个 id 是不是存在于 map 里就行了，相当于是一种 lazy 操作，因为动态维护 queue 队列里的顺序比较麻烦。

当然因为这个 lazy 操作会出现一种情况，就是 queue 里的 id 数和 map 里的 item 数不一致，那么就需要 `Resync()` 方法来进行同步了。它会直接遍历比较 queue 和 map，把不在 queue 里的 item 全部加到 queue 的末尾。

```go
// pkg/client/cache/fifo.go

// Resync will touch all objects to put them into the processing queue
func (f *FIFO) Resync() error {
    f.lock.Lock()
    defer f.lock.Unlock()

    inQueue := sets.NewString()
    for _, id := range f.queue {
        inQueue.Insert(id) // queue 里现在有的所有id
    }
    for id := range f.items { // 遍历map里的所有key,如果不在queue里就扔到queue末尾曲
        if !inQueue.Has(id) { 
            f.queue = append(f.queue, id)
        }
    }
    if len(f.queue) > 0 {
        f.cond.Broadcast()
    }
    return nil
}
```

最重要的是 Pop 方法的实现：

```go
// pkg/client/cache/fifo.go

func (f *FIFO) Pop(process PopProcessFunc) (interface{}, error) {
    f.lock.Lock()
    defer f.lock.Unlock()
    for {
        for len(f.queue) == 0 {
            f.cond.Wait()
        }
        id := f.queue[0]
        f.queue = f.queue[1:] // 从队列里拿出第一个item的id
        if f.initialPopulationCount > 0 {
            f.initialPopulationCount--
        }
        item, ok := f.items[id]
        if !ok {
            // Item may have been deleted subsequently.
            continue
        }
        // 如果id在map里不存在，那么跳过它
        delete(f.items, id) // 删掉map里对应的item
        err := process(item) // process方法会阻塞这个Pop方法直到这个item变为ready状态
        if e, ok := err.(ErrRequeue); ok {
            f.addIfNotPresent(id, item)
            err = e.Err
        }// 如果出现错误再次放回队列里
        return item, err
    }
}
```
Pop 方法里会从 queue 中拿出第一个 item 的 id，检查它是不是在 map 里，然后调用 process() 方法判断它是不是 ready 状态，最后返回这个 item。

阅读这里比较疑惑的是，实际 k8s 代码中貌似并没有实现这个 **process** 方法，可以看到在 `pkg/client/cache/fifo.go` 中有一个 Pop 方法的 helper，对方法进行了封装，而在封装中可以看到，传入的 `process()` 方法是个匿名函数，只是简单的把传入的 item 赋值给外部的 result 然后返回，并没有 check 这个 item 是否 ready。

```go
// pkg/client/cache/fifo.go

func Pop(queue Queue) interface{} {
    var result interface{}
    queue.Pop(func(obj interface{}) error {
        result = obj
        return nil
    })
    return result
}
```

最后需要提一下的就是这个 `keyFunc` 了。从 FIFO 类的定义上看，它是用于获取一个 item 的 key 的方法。而在 FIFO 实例化时，实际上传入的是 `MetaNamespaceKeyFunc()` 方法，获取 Pod 的 meta 信息里 `namespace/name` 或者 `name`。

```go
//pkg/client/cache/store.go

func MetaNamespaceKeyFunc(obj interface{}) (string, error) {
    if key, ok := obj.(ExplicitKey); ok {
        return string(key), nil
    }
    meta, err := meta.Accessor(obj)
    if err != nil {
        return "", fmt.Errorf("object has no meta: %v", err)
    }
    if len(meta.GetNamespace()) > 0 {
        return meta.GetNamespace() + "/" + meta.GetName(), nil
    }
    return meta.GetName(), nil
}
```

### DeltaFIFO Queue

在 k8s 的 cache 中还提供了一种叫做 **DeltaFIFO** 的队列，相比于 FIFO 队列，它的作用不一样，它是用于储存每个 Pod 的事件（包括 Delete, Update, Add）。这个方法是在 [ISSUE#4877](https://github.com/kubernetes/kubernetes/issues/4877) 中提出的，因为某些情况下控制器需要知道调度器中队列的操作事件，并且添加 watcher 来针对事件来做出响应，这里也简单的过一下。

首先 delta 的类型定义有 `Added`，`Updated`，`Deleted`，`Sync` 这四种，分别是在队列进行添加、更新、删除和同步操作时加入的事件。

```go
// pkg/client/cache/delta_fifo.go

const (
    Added   DeltaType = "Added"
    Updated DeltaType = "Updated"
    Deleted DeltaType = "Deleted"
    // The other types are obvious. You'll get Sync deltas when:
    //  * A watch expires/errors out and a new list/watch cycle is started.
    //  * You've turned on periodic syncs.
    // (Anything that trigger's DeltaFIFO's Replace() method.)
    Sync DeltaType = "Sync"
)
```

Delta 对象存储了**修改的类型（Type）**和**修改在哪个对象上（Object)**。

```go
// pkg/client/cache/delta_fifo.go

type Delta struct {
    Type   DeltaType
    Object interface{}
}
```

同样这个队列和 FIFO 类似，也是用 map 存 Deltas，用 queue 存 id。稍微不同的地方上，对这个队列的 `Add()`，`Update()`， `Delete()` 操作是把相应的 Delta **添加** 到队列里去，也就是说这个队列始终是在往里添加东西（Delta)，而外部的 Watcher 则在不断的从这个 DeltaFIFO 中取 Delta 来处理。也就是我们常说的 **Producer/Consumer** 的模型。

## 总结

和之前看的 Mesos 中的 DRF 算法不同，k8s 中使用 FIFO 队列来维护每个 Pod 进入调度器的顺序。当然 Mesos 用 DRF 也是考虑在两层调度框架上进行公平调度，而 k8s 只关心进来的 Pod 的先后顺序。

再和上篇文章说到的内容结合，k8s 的调度实际上是基于**先后顺序（FIFO），节点过滤（predicates）和节点打分（prioritizers）**来做到从 Pod 到 Node 的调度过程。

最后，调度器中 FIFO 当然也是最简单和最常使用的队列模式，或许之后可以为 k8s 增加更多模式的调度器。