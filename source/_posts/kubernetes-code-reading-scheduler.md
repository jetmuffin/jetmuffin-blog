title: kubernetes源码阅读 - scheduler
date: 2016-11-24 19:50:52
tags: [kubernetes, scheduler]
categories: 基础架构
thumbnail: http://cdn.jetmuffin.com/posts/kubernetes.jpg
---

阅读 scheduler 部分的源码我们从控制流的低端向上延伸去阅读。

![k8s](http://cdn.jetmuffin.com/posts/kubernetes.jpg)

<!--more-->

## Scheduler 入口

首先定位到 `plugin/cmd/scheduler.go` 这个文件。

```go
// plugin/cmd/scheduler.go

func main() {
    s := options.NewSchedulerServer()
    s.AddFlags(pflag.CommandLine) // 解析参数

    flag.InitFlags()
    logs.InitLogs()
    defer logs.FlushLogs()

    verflag.PrintAndExitIfRequested()

    app.Run(s) // 启动调度器
}
```

从上面这段代码可以看出这是从 command 启动 scheduler 的入口，它解析了 scheduler 的参数，同时调用了 `app.Run(s)`来启动 scheduler。

按着这个走向找到 `app.Run(s)` 的具体实现：

```go
// plugin/cmd/kube-scheduler/app/server.go

// Run runs the specified SchedulerServer.  This should never exit.
func Run(s *options.SchedulerServer) error {
  //...

    sched := scheduler.New(config) // 新建调度器

    run := func(_ <-chan struct{}) {
        sched.Run() // 正式启动调度器入口
        select {}
    }

    //...
}
```

可以看到在这段代码片段中，通过 `sched := scheduler.New(config)` 新建了一个 scheduler 对象，并且调用它的 `Run()` 成员方法，同时需要特意留意 `config`。继续追踪到 scheduler 类的定义中取：

```go
// plugin/pkg/scheduler/scheduler.go

// Run begins watching and scheduling. It starts a goroutine and returns immediately.
func (s *Scheduler) Run() {
    go wait.Until(s.scheduleOne, 0, s.config.StopEverything)
}
```

在 `Scheduler.Run()` 方法中，Scheduler 通过 `wait.Until()` 工具，不停得调用 `Scheduler.scheduleOne()` 方法直到收到停止信号。我们接着看 `scheduleOne()` 方法：

```go
// plugin/pkg/scheduler/scheduler.go

func (s *Scheduler) scheduleOne() {
    pod := s.config.NextPod() // 获取下一个待调度的pod

    glog.V(3).Infof("Attempting to schedule pod: %v/%v", pod.Namespace, pod.Name)
    start := time.Now()
    dest, err := s.config.Algorithm.Schedule(pod, s.config.NodeLister) // 调用算法进行调度
    if err != nil {
        glog.V(1).Infof("Failed to schedule pod: %v/%v", pod.Namespace, pod.Name)
        s.config.Error(pod, err)
        s.config.Recorder.Eventf(pod, api.EventTypeWarning, "FailedScheduling", "%v", err)
        s.config.PodConditionUpdater.Update(pod, &api.PodCondition{
            Type:   api.PodScheduled,
            Status: api.ConditionFalse,
            Reason: api.PodReasonUnschedulable,
        })
        return
    }
    metrics.SchedulingAlgorithmLatency.Observe(metrics.SinceInMicroseconds(start))

    // ...
}
```

Scheduler 首先通过 `config.NextPod()` 来获取到当前要进行调度的一个 pod，然后调用 `config.Algorithm.Schedule()` 方法来对这个 pod 实际进行调度。从这种写法上我们可以猜测到 Algorithm 这儿用的是工厂模式。若根据该算法调度成功，那么将这个时间进行记录。否则调用 `config.Error()` 来进行错误处理。

这里有三个比较重点的内容：

- config.NextPod()
- config.Algorithm.schedule()
- config.Error()

它们均来自 `Config` 类，那么我们先看这个类，再接下去探索这两个方法。`Config` 类的定义也在这个文件中：

```go
// plugin/pkg/scheduler/scheduler.go

type Config struct {
    // It is expected that changes made via SchedulerCache will be observed
    // by NodeLister and Algorithm.
    SchedulerCache schedulercache.Cache
    NodeLister     algorithm.NodeLister
    Algorithm      algorithm.ScheduleAlgorithm // 调度算法
    Binder         Binder
    // PodConditionUpdater is used only in case of scheduling errors. If we succeed
    // with scheduling, PodScheduled condition will be updated in apiserver in /bind
    // handler so that binding and setting PodCondition it is atomic.
    PodConditionUpdater PodConditionUpdater

    // NextPod should be a function that blocks until the next pod
    // is available. We don't use a channel for this, because scheduling
    // a pod may take some amount of time and we don't want pods to get
    // stale while they sit in a channel.
    NextPod func() *api.Pod // 获取下一个pod的抽象方法

    // Error is called if there is an error. It is passed the pod in
    // question, and the error
    Error func(*api.Pod, error) // 处理调度出现的错误

    // Recorder is the EventRecorder to use
    Recorder record.EventRecorder

    // Close this to shut down the scheduler.
    StopEverything chan struct{} // 停止信号
}
```

`Config` 类传入了 scheduler 必要的各种参数，相当于原来隶属于 scheduler 的成员变量通过它传入。`Config` 类在 `plugin/pkg/scheduler/factory` 中被实例化，相应的抽象方法也被传入实际的引用方法。

```go
  // plugin/pkg/scheduler/factory
  
  // ...
  algo := scheduler.NewGenericScheduler(f.schedulerCache, predicateFuncs, predicateMetaProducer, priorityConfigs, priorityMetaProducer, extenders)
  // ...
  
    return &scheduler.Config{
        SchedulerCache: f.schedulerCache,
        // The scheduler only needs to consider schedulable nodes.
        NodeLister:          f.NodeLister.NodeCondition(getNodeConditionPredicate()),
        Algorithm:           algo,
        Binder:              &binder{f.Client},
        PodConditionUpdater: &podConditionUpdater{f.Client},
        NextPod: func() *api.Pod {
            return f.getNextPod()
        },
        Error:          f.makeDefaultErrorFunc(&podBackoff, f.PodQueue),
        StopEverything: f.StopEverything,
    }, nil
```

## 获取待调度的 Pod

首先看 `NextPod` 参数，从类 `Config` 的定义中的注释可以看到，这个参数是一个获取下一个 pod，并且阻塞该方法直到下一个 pod 可用为止。并且它的实现没有用 chanel，原因是一旦放到 chanel 里，这个 pod 便无法修改。这个 参数是在实例化时传入的获取下一个 pod 的方法。传入的代码也在这个文件中：

```go
// plugin/pkg/scheduler/factory

func (f *ConfigFactory) getNextPod() *api.Pod {
    for {
        pod := cache.Pop(f.PodQueue).(*api.Pod)
        if f.responsibleForPod(pod) {
            glog.V(4).Infof("About to try and schedule pod %v", pod.Name)
            return pod
        }
    }
}
```

从这段代码可以看出，获取下一个待调度的 pod 似乎是从一个队列中 pop 出一个 pod 来实现的。使用的来源是 `pkg/client/cache`，继续追踪：

```go
// pkg/client/cache

func Pop(queue Queue) interface{} {
    var result interface{}
    queue.Pop(func(obj interface{}) error {
        result = obj
        return nil
    })
    return result
}
```

这个方法是个代理方法，是对 `Queue` 类中的 `Pop()` 方法进行了一层封装，于是自然看到对应的 `Queue.Pop()`，它是在类 `FIFO` 中的，那么这块儿也逐渐明了了:

```go
// pkg/client/cache/fifo.go

// Pop waits until an item is ready and processes it. If multiple items are
// ready, they are returned in the order in which they were added/updated.
// The item is removed from the queue (and the store) before it is processed,
// so if you don't successfully process it, it should be added back with
// AddIfNotPresent(). process function is called under lock, so it is safe
// update data structures in it that need to be in sync with the queue.
func (f *FIFO) Pop(process PopProcessFunc) (interface{}, error) {
    f.lock.Lock()
    defer f.lock.Unlock()
    for {
        for len(f.queue) == 0 {
            f.cond.Wait()
        }
        id := f.queue[0]
        f.queue = f.queue[1:]
        if f.initialPopulationCount > 0 {
            f.initialPopulationCount--
        }
        item, ok := f.items[id]
        if !ok {
            // Item may have been deleted subsequently.
            continue
        }
        delete(f.items, id)
        err := process(item)
        if e, ok := err.(ErrRequeue); ok {
            f.addIfNotPresent(id, item)
            err = e.Err
        }
        return item, err
    }
}
```

这个方法算是这条路径的最后一个追踪的节点了。总结一下这条路径：

1. kubernetes 的 cache 中维护了一个先进先出的队列 `FIFO`，它不同于 golang 中的 chanel，它支持对队列中的元素进行更新、删除等操作。
2. 这个队列的 `FIFO.Pop()` 方法会队首元素（一个 pod）到达 ready 状态，然后将它弹出，否则阻塞该方法。
3. `Scheduler` 类初始化时使用参数 `Config`，`Config` 中的 `NextPod()` 默认使用 `FIFO.Pop()`，当 `Scheduler` 需要进行调度时使用该方法得到下一个待调度的 pod。

## 使用调度算法进行实时调度

回到 `Config` 类的第二个重要参数 `Algorithm`,它是对 pod 进行调度的实体：

```go
// plugin/pkg/scheduler/factory/factory.go

algo := scheduler.NewGenericScheduler(f.schedulerCache, predicateFuncs, predicateMetaProducer, priorityConfigs, priorityMetaProducer, extenders)
```

可以看到默认情况下，`Algorithm` 是用的通用调度器，接着看这个 `GenericScheduler`：

```go
// plugin/pkg/scheduler/generic_scheduler.go

// Schedule tries to schedule the given pod to one of node in the node list.
// If it succeeds, it will return the name of the node.
// If it fails, it will return a Fiterror error with reasons.
func (g *genericScheduler) Schedule(pod *api.Pod, nodeLister algorithm.NodeLister) (string, error) {
    var trace *util.Trace
    if pod != nil {
        trace = util.NewTrace(fmt.Sprintf("Scheduling %s/%s", pod.Namespace, pod.Name))
    } else {
        trace = util.NewTrace("Scheduling <nil> pod")
    }
    defer trace.LogIfLong(100 * time.Millisecond)

    nodes, err := nodeLister.List() // 获取所有节点
    if err != nil {
        return "", err
    }
    if len(nodes) == 0 {
        return "", ErrNoNodesAvailable
    }

    // Used for all fit and priority funcs.
    err = g.cache.UpdateNodeNameToInfoMap(g.cachedNodeInfoMap)
    if err != nil {
        return "", err
    }

    // TODO(harryz) Check if equivalenceCache is enabled and call scheduleWithEquivalenceClass here

    trace.Step("Computing predicates")
    filteredNodes, failedPredicateMap, err := findNodesThatFit(pod, g.cachedNodeInfoMap, nodes, g.predicates, g.extenders, g.predicateMetaProducer)
    if err != nil {
        return "", err
    }

    if len(filteredNodes) == 0 {
        return "", &FitError{
            Pod:              pod,
            FailedPredicates: failedPredicateMap,
        }
    }

    trace.Step("Prioritizing")
    metaPrioritiesInterface := g.priorityMetaProducer(pod, g.cachedNodeInfoMap)
    priorityList, err := PrioritizeNodes(pod, g.cachedNodeInfoMap, metaPrioritiesInterface, g.prioritizers, filteredNodes, g.extenders)
    if err != nil {
        return "", err
    }

    trace.Step("Selecting host")
    return g.selectHost(priorityList)
}
```

可以看到，GenericScheduler 进行调度为以下过程：

1. 从 `NodeList` 获取所有的节点
2. 通过所给的断言函数（`predicates`）对所有节点进行过滤，选出符合的节点（`findNodesThatFit()`）
3. 根据所给的打分函数（`prioritizers`）对过滤后的节点进行排序（`PrioritizeNodes()`）
4. 通过 round-robin 方式依次获取分数最高的节点来运行 pod。

## 调度失败

在 `Config` 类的 `Error` 参数中传入了调度失败的处理方法 `makeDefaultErrorFunc()` 对调度失败的 pod 进行重调度处理。

```go
// plugin/pkg/scheduler/factory/factory.go

func (factory *ConfigFactory) makeDefaultErrorFunc(backoff *podBackoff, podQueue *cache.FIFO) func(pod *api.Pod, err error) {
    return func(pod *api.Pod, err error) {
        if err == scheduler.ErrNoNodesAvailable {
            glog.V(4).Infof("Unable to schedule %v %v: no nodes are registered to the cluster; waiting", pod.Namespace, pod.Name)
        } else {
            glog.Errorf("Error scheduling %v %v: %v; retrying", pod.Namespace, pod.Name, err)
        }
        backoff.gc() // 清空backoff列表
        // Retry asynchronously.
        // Note that this is extremely rudimentary and we need a more real error handling path.
        go func() {
            defer runtime.HandleCrash()
            podID := types.NamespacedName{
                Namespace: pod.Namespace,
                Name:      pod.Name,
            }

            entry := backoff.getEntry(podID)
            if !entry.TryWait(backoff.maxDuration) {
                glog.Warningf("Request for pod %v already in flight, abandoning", podID)
                return
            }
            // Get the pod again; it may have changed/been scheduled already.
            getBackoff := initialGetBackoff
            for {
                pod, err := factory.Client.Core().Pods(podID.Namespace).Get(podID.Name)
                if err == nil {
                    if len(pod.Spec.NodeName) == 0 {
                        podQueue.AddIfNotPresent(pod) // 重新将pod加入调度队列
                    }
                    break
                }
                if errors.IsNotFound(err) {
                    glog.Warningf("A pod %v no longer exists", podID)
                    return
                }
                glog.Errorf("Error getting pod %v for retry: %v; retrying...", podID, err)
                if getBackoff = getBackoff * 2; getBackoff > maximalGetBackoff {
                    getBackoff = maximalGetBackoff
                }
                time.Sleep(getBackoff)
            }
        }()
    }
}
```

从该处理方法看出，一旦出现调度失败，k8s 并不会阻塞调度过程，而是将调度失败的 pod 扔到这个失败处理方法里，而这个方法是通过异步的方式进行重试。k8s 从 backoff 这个列表中拿出指定的 pod，然后将它再次放到 `FIFO` 队列里，期间如果继续失败，再扔进 backoff 里异步等待处理。

## 总结

k8s 的代码量非常庞大，需要快速定位源码比较困难，而采用这种根据从入口不断深入的阅读方式会减少很多工作量，只关心一部分的代码。当然这也无法从一个宏观的角度去看整个代码的设计，但是对于当前的工作已经足够。
