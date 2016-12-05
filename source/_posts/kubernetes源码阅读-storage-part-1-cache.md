title: kubernetes源码阅读 - storage part 1 - cache
date: 2016-12-04 21:28:54
tags: [kubernetes,cache,storage]
categories: [基础架构,容器管理]
thumbnail: http://cdn.jetmuffin.com/posts/gce.png
---
kubernetes 的 scheduler 维护了一个先进先出的队列，而这个队列的事实上是一种 `Storage`，只不过它重载了 Storage 中的各种方法。那么 kubernetes 中的存储是如何设计的呢？

<!--more-->

```go
// pkg/client/cache/fifo.go

type Queue interface {
	Store // Queue事实上也是一种Storage，而在FIFOQueue中具体对Add,List,Delete等方法进行了重写

	Pop(PopProcessFunc) (interface{}, error)

	AddIfNotPresent(interface{}) error

	// Return true if the first batch of items has been popped
	HasSynced() bool
}
```

## Cache

在 `pkg/client/cache` 包里实现了 cache 的一些基本结构，其中最核心的，也是最顶层的结构是 `Store` 接口：

```go
// pkg/client/cache/store.go

type Store interface {
	Add(obj interface{}) error // 增
	Update(obj interface{}) error // 改
	Delete(obj interface{}) error // 删
	List() []interface{} // 列举
	ListKeys() []string // 列举 key
	Get(obj interface{}) (item interface{}, exists bool, err error) // 查
	GetByKey(key string) (item interface{}, exists bool, err error) // 按key查

	// Replace will delete the contents of the store, using instead the
	// given list. Store takes ownership of the list, you should not reference
	// it after calling this function.
	Replace([]interface{}, string) error // 替换
	Resync() error // 同步
}
```

Store 接口定义了 cache 中的所有数据类型的最通用的一层抽象，包括对 cache 存储的增删改查列举等操作。Store 具体的接口实现包括前面提到的 `FIFO`，`DeltaFIFO`，以及后面会提到的 `ThreadSafeStore`，`UnDeltaStore` 和 `ExpirationCache`。

### ThreadSafeStore

`ThreadSafeStore` 是 k8s 中用的最多，也是默认的 cache 存储。它允许对数据并行的访问，同时使用加锁的机制保证数据在多线程中是线程安全的。

```go
// pkg/client/cache/thread_safe_store.go

type threadSafeMap struct {
	lock  sync.RWMutex // 锁
	items map[string]interface{} // 存储数据的map

	// indexers maps a name to an IndexFunc
	indexers Indexers // 存储Indexer的map
	// indices maps a name to an Index
	indices Indices // 存储Index的map
}
```

`ThreadSafeStore` 的实现上也比较简单，数据放在 map 的数据结构里，同时使用 `sync.RWMutex` 对这个 map 的 Add，Update，Delete，Get 等操作加读锁和写锁。此外，它还允许对同一作用的 items 加一个共同的索引方便直接获取，而这个索引放在 `indices` 里，获得索引的函数放在 `indexers` 里。

```go
// pkg/client/cache/thread_safe_store.go

func (c *threadSafeMap) Index(indexName string, obj interface{}) ([]interface{}, error) {
	c.lock.RLock()
	defer c.lock.RUnlock()

	indexFunc := c.indexers[indexName] // 通过给的索引名称拿到索引函数
	if indexFunc == nil {
		return nil, fmt.Errorf("Index with name %s does not exist", indexName)
	}

	indexKeys, err := indexFunc(obj) // 用索引函数拿到索引的keys
	if err != nil {
		return nil, err
	}
	index := c.indices[indexName]

	// need to de-dupe the return list.  Since multiple keys are allowed, this can happen.
	returnKeySet := sets.String{}
	for _, indexKey := range indexKeys {  // 对索引的keys进行去重
		set := index[indexKey]
		for _, key := range set.UnsortedList() {
			returnKeySet.Insert(key)
		}
	}

	list := make([]interface{}, 0, returnKeySet.Len())
	for absoluteKey := range returnKeySet {
		list = append(list, c.items[absoluteKey]) // 根据keys拿到实际的items放到list里
	}
	return list, nil
}
```

### UndeltaStore

`UndeltaStore` 在 `Store` 上做了一层封装，在实例化时传入了一个回调函数 `PushFunc`，每当 `Store` 进行了 Add，Update，Delete 等操作，会调用 `PushFunc` 来告知这个数据的操作已经完成，可以进行后续的工作。

```go
// pkg/client/cache/undelta_store.go

func (u *UndeltaStore) Add(obj interface{}) error {
	if err := u.Store.Add(obj); err != nil {
		return err
	}
	u.PushFunc(u.Store.List())
	return nil
}
```

在实现上其实也很简单，在每个操作的最后调用一次传入的 `PushFunc()`。

### ExpirationCache

`ExpirationCache` 同样是对 `Store` 做了一层的封装，和 `UndeltaStore` 不同的是它是在 `ThreadSafeStore` 上进行的封装，实现了带过期期限的 cache 功能。

```go
// pkg/client/cache/expiration_cache.go

type ExpirationCache struct {
	cacheStorage     ThreadSafeStore // 实际存储的storage
	keyFunc          KeyFunc
	clock            clock.Clock // 标记创建时间的时间戳
	expirationPolicy ExpirationPolicy // 失效策略
	// expirationLock is a write lock used to guarantee that we don't clobber
	// newly inserted objects because of a stale expiration timestamp comparison
	expirationLock sync.Mutex // 失效检查的锁
}
```

当 `ExpirationCache` 中的每个 item 被创建的时候会为其打上时间戳，而这个 item 在查询的时候会检查这个时间戳到目前的时间间隔是否超过了失效的阈值，若超过了则返回 `nil` 并把这个 item 从 map 里删掉。（这里相当于做了一个 lazy 操作，如果 item 失效了但是一直没有查询，那么它会一直在 map 里待着）。

默认使用的失效策略比较简单，就是给定一个时间范围阈值 `TTL(TimeToLive)`，然后判断这个时间戳到现在的时间距离是否大于这个阈值。

```go
// pkg/client/cache/expiration_cache.go
type TTLPolicy struct {
	//	 >0: Expire entries with an age > ttl
	//	<=0: Don't expire any entry
	Ttl time.Duration // 过期阈值

	// Clock used to calculate ttl expiration
	Clock clock.Clock
}

func (p *TTLPolicy) IsExpired(obj *timestampedEntry) bool {
	return p.Ttl > 0 && p.Clock.Since(obj.timestamp) > p.Ttl // 判断是否失效（间隔是否大于TTL）
}
```

相比于 `ThreadSafeStore`，`ExpirationCache` 在使用时还需要检查 item 是不是失效了，而这个检查的过程也是加锁的，而我们知道 `ThreadSafeStore` 自身本来就是带锁的操作的，所以它的操作会比 `ThreadSafeStore` 慢一些。

### Others

除了上面提到的几种 cache 中用到的数据结构外，cache 包里还提供了一些其他的类来进行辅助工作：

- Indexer（`pkg/client/cache/index.go`）允许用多种方法对 items 进行过滤（索引）
- Lister（`pkg/client/cache/lister.go`）提供了各种列举 items 的方法
- MutationDetection（`pkg/client/cache/mutation_dectector.go`）检测某个 item 是否发生变化了
- ...

## Reflector

cache 包里的各个类定义了不同场景下 k8s 使用的数据结构，我们也知道 k8s 里实际存储 pods 等资源是放在 `ThreadSafeStore` 里的，那么 k8s 是如何将 APIServer 里的 RESTful API 和 cache 相联系起来的呢？

在 `pkg/client/cache/reflector.go` 里，k8s 提供了一种叫做 `Reflector` 的类，用于作为 APIServer 和 cache 之间的桥梁。对每一类 Resource 为它创建一个从 RESTful Resource 到 cache item 的 Reflector。

```go
// pkg/client/cache/reflector.go

type Reflector struct {
	// name identifies this reflector. By default it will be a file:line if possible.
	name string

	// The type of object we expect to place in the store.
	expectedType reflect.Type // item的类型
	// The destination to sync up with the watch source
	store Store // item 的实际存储
	// listerWatcher is used to perform lists and watches.
	listerWatcher ListerWatcher // 用于监听item变化的watcher
	// period controls timing between one watch ending and
	// the beginning of the next one.
	period       time.Duration //watch的间隔
	resyncPeriod time.Duration
	// now() returns current time - exposed for testing purposes
	now func() time.Time
	// lastSyncResourceVersion is the resource version token last
	// observed when doing a sync with the underlying store
	// it is thread safe, but not synchronized with the underlying store
	lastSyncResourceVersion string
	// lastSyncResourceVersionMutex guards read/write access to lastSyncResourceVersion
	lastSyncResourceVersionMutex sync.RWMutex
}
```

Reflector 有两种允许方式：

- `Run()`
- `RunUntil(stopCh <-chan struct{})`

两者的区别在于后者传入了一个 stopCh，当收到停止信号会停止这个 `Run()` 方法，而前者则是 Run forever 的。而两者的方法的主题都是一致的，都在启动了一个 goroutine 去一直执行 `ListAndWatch()` 方法。

```go
// pkg/client/cache/reflector.go
func (r *Reflector) Run() {
	glog.V(3).Infof("Starting reflector %v (%s) from %s", r.expectedType, r.resyncPeriod, r.name)
	go wait.Until(func() {
		if err := r.ListAndWatch(wait.NeverStop); err != nil { // 一直执行ListAndWatch
			utilruntime.HandleError(err)
		}
	}, r.period, wait.NeverStop)
}
```

那么来看一看这个 `ListAndWatch()` 是做什么的：

```go
// pkg/client/cache/reflector.go

//...
w, err := r.listerWatcher.Watch(options) // 获得一个watch.Interface
if err != nil {
	switch err {
	case io.EOF:
		// watch closed normally
	case io.ErrUnexpectedEOF:
		glog.V(1).Infof("%s: Watch for %v closed with unexpected EOF: %v", r.name, r.expectedType, err)
	default:
		utilruntime.HandleError(fmt.Errorf("%s: Failed to watch %v: %v", r.name, r.expectedType, err))
	}
	// If this is "connection refused" error, it means that most likely apiserver is not responsive.
	// It doesn't make sense to re-list all objects because most likely we will be able to restart
	// watch where we ended.
	// If that's the case wait and resend watch request.
	if urlError, ok := err.(*url.Error); ok {
		if opError, ok := urlError.Err.(*net.OpError); ok {
			if errno, ok := opError.Err.(syscall.Errno); ok && errno == syscall.ECONNREFUSED {
				time.Sleep(time.Second)
				continue
			}
		}
	}
	return nil
}

if err := r.watchHandler(w, &resourceVersion, resyncerrc, stopCh); err != nil { // 处理这个 watch.Interface
	if err != errorStopRequested {
		glog.Warningf("%s: watch of %v ended with: %v", r.name, r.expectedType, err)
	}
	return nil
}
//...
```

在 `ListAndWatch()` 中前面先开了一个独立的 goroutine 去同步 Store 这里就不细说了。核心的内容在后面，首先是 `r.listerWatcher.Watch(options)` 获取到一个 `watch.Interface` 对象，然后对这个对象进行处理，即 `r.watchHandler()`。

首先看这个 `watch.Interface`，寻找引用它的路径可以按着 `pkg/client/cache/listwatch.go: ListerWatcher.Watch()` -> `pkg/client/cache/listwatch.go: ListWatch.WatchFunc` -> `pkg/client/cache/listwatch.go: NewListWatchFromClient()` 的一个思路去看。

那么最终关注这个 `NewListWatchFromClient()` 方法，它事实上就是连接着 API 的那一头，传入了 API 的一个 Resource，而 `ListerWatcher` 的 `Watch()` 方法就是这个 Resource 中的 `Watch()` 方法。具体实例化是在每个资源的包中，例如 `pkg/kubelet/config/apiserver.go` 中：

```go
// pkg/kubelet/config/apiserver.go

func NewSourceApiserver(c *clientset.Clientset, nodeName types.NodeName, updates chan<- interface{}) {
	lw := cache.NewListWatchFromClient(c.Core().RESTClient(), "pods", v1.NamespaceAll, fields.OneTermEqualSelector(api.PodHostField, string(nodeName)))
	newSourceApiserverFromLW(lw, updates)
}
```

这里为 pods 创建了一个 RESTClient，同时用这个 Client 作为参数创建一个 ListWatcher，ListAndWatch 中的 Watch 方法就是 Client 中的 Watch 方法。

那么搞清楚了这一头了以后看一看检测到变化以后 Reflector 是怎么处理的。在 ListAndWatch 方法中还有一个重要的方法 `watchHandler()` 方法：

```go
// pkg/client/cache/reflector.go

func (r *Reflector) watchHandler(w watch.Interface, resourceVersion *string, errc chan error, stopCh <-chan struct{}) error {
  	//...
	for {
		select {
		case <-stopCh:
			return errorStopRequested
		case err := <-errc:
			return err
		case event, ok := <-w.ResultChan(): // 从ResultChan中拿出一个event
			if !ok {
				break loop
			}
			if event.Type == watch.Error {
				return apierrs.FromObject(event.Object)
			}
			if e, a := r.expectedType, reflect.TypeOf(event.Object); e != nil && e != a {
				utilruntime.HandleError(fmt.Errorf("%s: expected type %v, but watch event object had type %v", r.name, e, a))
				continue
			}
			meta, err := meta.Accessor(event.Object) //用event.Object获取meta信息
			if err != nil {
				utilruntime.HandleError(fmt.Errorf("%s: unable to understand watch event %#v", r.name, event))
				continue
			}
			newResourceVersion := meta.GetResourceVersion()
			switch event.Type { // 根据event的不同类型对cache做不同处理
			case watch.Added:
				r.store.Add(event.Object)
			case watch.Modified:
				r.store.Update(event.Object)
			case watch.Deleted:
				// TODO: Will any consumers need access to the "last known
				// state", which is passed in event.Object? If so, may need
				// to change this.
				r.store.Delete(event.Object)
			default:
				utilruntime.HandleError(fmt.Errorf("%s: unable to understand watch event %#v", r.name, event))
			}
			*resourceVersion = newResourceVersion
			r.setLastSyncResourceVersion(newResourceVersion)
			eventCount++
		}
	}
  //...
}
```

`watchHandler()` 的核心是这个 loop，Reflector 会从 watch.Interface 中拿到一个叫做 ResultChan 的 chanel，watch.Interface 在 API 的那一头会根据 HTTP Request 源源不断往这个 chanel 里扔进去事件 Event（`pkg/watch/watch.go: Event`），而 Reflector 从 chanel 的这一头不断的拿出这些事件，获取他们的 meta 信息，再根据事件的不同类型对 cache 做增删改查操作。

也就是说 k8s 对各种操作事实上是统一通过这个 chanel 里以事件的形式市传过来的，这也解释了为什么 cache 和 Store 中的实现所有的方法都是传进去一个 object 的 interface。

## 总结

到这里所有的路径都打通了，再最后理一遍 k8s 是如何从 API 到 cache 的 Store 的：

1. APIServer 为每类资源（例如 pods）创建一个 RESTClient（client）用于处理诸如 POST，PUT，GET 的 RESTful HTTP requsts
2. APIServer 用 client 创建一个 ListWatcher（lw）用于监控每类资源的变化
3. APIServer 用 lw，指定的 store，创建一个 Reflector（reflector），将这个变化作用到 store 上
4. 当发生 HTTP Requests 时，client 的 Watch 方法会把资源和资源变化类型打包成 event 放进 lw 的 ResultChan 中
5. reflector 从 ResultChan 里拿出 event，根据 event.EventType 把 event.Object 作用到 store 里去

整个过程中用了许多的 goroutine 来完成这么一件事。相比于 k8s 的实现，常见的 webapp 的处理相对就简单了好多，也可以看出 k8s 在设计上的精妙。