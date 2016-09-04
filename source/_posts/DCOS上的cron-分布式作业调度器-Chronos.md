layout: blog
title: DCOS上的cron 分布式作业调度器 Chronos
date: 2015-12-28 21:34:27
tags: [mesos,chronos,批处理,脚本]
categories: 基础架构
---
Chronos是Aribnb公司开发的替代`cron`的Mesos框架。它是一个运行在`Apache Mesos`上的分布式的，高容错的任务编排、定时任务的框架。

<!-- more -->

Chronos作为DCOS中的一部分，它默认所处的环境下存在任务的环境。例如Chronos会执行Hadoop任务脚本，即使执行的slave上并没有Hadoop环境（但是这样会导致任务的失败）。此外，Chronos支持任务脚本传输文件到远程机器上，并执行文件，同时接受异步的回调，通知Chronos任务成功或是失败。比较重要的一点是，Chronos同样支持在`Docker`容器中进行执行任务。

## 架构

![architecture](http://7xpl2y.com1.z0.glb.clouddn.com/chronos.png)

Chronos通过Mesos Master下的`Scheduler`选择指定slave执行job，来完成相应的task。而在Chronos中并不会对环境进行选择，而只是对从Mesos Master接受的`Resource offers`进行筛选。

## JobGraph

Chronos中比较重要的一点是利用JobGraph来确定各个job之间的依赖关系，以确定任务执行的先后顺序。

Chronos中根据JobGraph计算出job执行的优先级，然后再根据当前的job执行情况，然后确定每个任务是否进行执行。

如图，我创建了2个任务:

![create-job](http://7xpl2y.com1.z0.glb.clouddn.com/QQ20151228-3%402x.png)

指定`sleep2`的父元素为`sleep`，则`sleep2`依赖于`sleep`，先后顺序也应该为先执行`sleep`然后是`sleep2`，如下图(绿色代表已经执行，白色代表未执行)

![job-run](http://7xpl2y.com1.z0.glb.clouddn.com/QQ20151228-2%402x.png)

而实际生产中JobGraph的图是非常复杂的，因此Chronos利用JobGraph能够很有效的进行任务的调度。

## 部分源码解析

这里主要分析framework部分的主要核心代码：

**resourceOffers()**

```scala
//该方法实现调用mesos的scala接口，进行offer的分配
  def resourceOffers(schedulerDriver: SchedulerDriver, receivedOffers: java.util.List[Offer]) {
    log.info("Received resource offers")
    import scala.collection.JavaConverters._

    val offers = receivedOffers.asScala.toList
    val offerResources = mutable.HashMap(offers.map(o => (o, Resources(o))).toSeq: _*)
    //生成可以执行的task(详细见下文)
    val tasksToLaunch = generateLaunchableTasks(offerResources)

    log.info("Declining unused offers.")
    
    // 创建一个Set保存已经使用的offer
    val usedOffers = mutable.HashSet(tasksToLaunch.map(_._3.getId.getValue): _*)
	
	//如果当前的offer已经被使用了（在usedOffers里出现），则decline掉
    offers.foreach(o => {
      if (!usedOffers.contains(o.getId.getValue))
        mesosDriver.get().declineOffer(o.getId, declineOfferFilters)
    })

    log.info(s"Declined unused offers with filter refuseSeconds=${declineOfferFilters.getRefuseSeconds} " +
      s"(use --${config.declineOfferDuration.name} to reconfigure)")
	
	//执行任务
    launchTasks(tasksToLaunch)

    // Perform a reconciliation, if needed.
    reconcile(schedulerDriver)
  }
```

**generateLaunchableTasks（）**

```scala
  def generateLaunchableTasks(offerResources: mutable.HashMap[Offer, Resources]): mutable.Buffer[(String, BaseJob, Offer)] = {
    val tasks = mutable.Buffer[(String, BaseJob, Offer)]()
    
	// 属性约束检查
    def checkConstraints(attributes: Seq[Protos.Attribute], constraints: Seq[Constraint]): Boolean = {
    	//遍历所有的属性约束
      constraints.foreach { c =>
        if (!c.matches(attributes)) {
          return false
        }
      }
      true
    }
	
	//尾递归注解
    @tailrec
    def generate() {
      taskManager.getTask match {
        case None => log.info("No tasks scheduled or next task has been disabled.\n")
        case Some((taskId, job)) =>
          if (runningTasks.contains(job.name)) {
          	// 若当前的task正在执行，则从taskManager里将其去除
            val deleted = taskManager.removeTask(taskId)
            log.warning("The head of the task queue appears to already be running: " + job.name + "\n")
            // 递归调用
            generate()
          } else {
            tasks.find(_._2.name == job.name) match {
              case Some((subtaskId, subJob, offer)) =>
                //若发现已经调度的任务出现在队列中，则将其删除
                val deleted = taskManager.removeTask(subtaskId)
                log.warning("Found job in queue that is already scheduled for launch with this offer set: " + subJob.name + "\n")
                //递归调用
                generate()
              case None =>
              	//生产当前任务所需的资源，并且判断每个offer的各个资源是否能够满足
                val neededResources = new Resources(job)
                offerResources.toIterator.find { ors =>
                  ors._2.canSatisfy(neededResources) && checkConstraints(ors._1.getAttributesList.asScala, job.constraints)
                } match {
                  case Some((offer, resources)) =>
                  	//若存在资源能够满足
                    // Subtract this job's resource requirements from the remaining available resources in this offer.	
                    //计算新资源，执行任务
                    resources -= neededResources
                    tasks.append((taskId, job, offer))
                    //继续递归
                    generate()
                  case None =>
                  	// 资源不足，则将task重新加回队列（taskManager）
                    val foundResources = offerResources.toIterator.map(_._2.toString()).mkString(",")
                    log.warning(
                      "Insufficient resources remaining for task '%s', will append to queue. (Needed: [%s], Found: [%s])"
                        .stripMargin.format(taskId, neededResources, foundResources)
                    )
                    taskManager.enqueue(taskId, job.highPriority)
                }
            }
          }
      }
    }
    generate()
    tasks
  }
```

从这段源码中可以看出来taskManaer事实上是一个task的`优先队列`。同时从generateLaunchableTasks()这个函数看出Chronos利用Scala的尾递归巧妙地处理了所有情况，保证调度器工作的执行。

## 总结

Chronos是一个非常好的Mesos框架，同时它的源码采用scala语言进行编写，函数式编程也让代码看起来非常的舒畅。由于笔者毕业设计打算写一个Mesos Framework，所以Chronos是一个很好的典范！