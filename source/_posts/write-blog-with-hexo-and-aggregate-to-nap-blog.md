title: 使用 Hexo 搭建个人博客并聚合到 NAP-blog
date: 2017-09-26 13:52:44
tags: [hexo,blog]
---

> Hexo is a fast, simple & powerful blog framework.

Hexo 是一个很流行的博客框架，组里的同学大多都使用 hexo 来撰写个人博客，刚好之前写了一个基于 webhook 的博客聚合，将组里所有人的博客自动合并到一个总的博客中。这篇文章主要讲一下如何从头搭建一个 hexo 博客，以及如何聚合到 NAP-blog 上。

<!--more-->

## 搭建 Hexo 博客

### 环境搭建

Hexo 是用 node.js 编写的框架，因此需要 `node` 环境，同时如果需要 hexo 部署到 github pages 上，还需要配置 `git` 环境，鉴于这两者配置相对简单，这里给出教程地址：

* [下载 node.js](https://nodejs.org/en/download/) / [安装 node.js](http://www.runoob.com/nodejs/nodejs-install-setup.html)
* [安装 git](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/00137396287703354d8c6c01c904c7d9ff056ae23da865a000)

安装完毕后，分别使用以下命令确定是否正确安装，安装的版本根据个人所需而定。

```bash
$ node -v
v4.4.0
$ npm -v
2.14.20
$ git --version
git version 2.11.0 (Apple Git-81)
```

### 安装 Hexo

首先需要在全局环境下安装 `hexo-cli`，在命令行下键入命令：

```bash
$ npm install hexo-cli -g
```

待安装完毕后，再使用确定 `hexo-cli` 是否正确安装完毕：

```bash
$ hexo -v
```

### 创建 Hexo 博客

找一个合适的路径，创建一个用于存放你的 hexo 文件和代码的文件夹，例如 `~/blog`，进入该文件夹后通过 `hexo init` 初始化 hexo 目录结构，再通过 `npm install` 安装 hexo 所需的所有依赖。

```bash
$ mkdir ~/blog
$ cd ~/blog
$ hexo init
INFO  Copying data to ~/blog-test
INFO  You are almost done! Don't forget to run 'npm install' before you start blogging with Hexo!
$ npm install
hexo@3.3.9 node_modules/hexo
├── abbrev@1.1.0
├── pretty-hrtime@1.0.3
├── hexo-front-matter@0.2.3
├── archy@1.0.0
├── titlecase@1.1.2
├── text-table@0.2.0
......
├── nunjucks@2.5.2 (asap@2.0.6, yargs@3.32.0, chokidar@1.7.0)
└── hexo-fs@0.2.2 (escape-string-regexp@1.0.5, graceful-fs@4.1.11, chokidar@1.7.0)
```

OK，到这一步一个全新的 hexo 博客已经搭建完毕了，这里只要用命令 `hexo s` 或 `hexo server` 即可运行搭建好的博客，博客运行的地址为 [http://localhost:4000](http://localhost:4000)。

```bash
$ hexo s
INFO  Start processing
INFO  Hexo is running at http://0.0.0.0:4000/. Press Ctrl+C to stop.
```

打开浏览器，键入运行的地址，可以看到已经可以查看博客的内容了。

![hexo-website](http://cdn.jetmuffin.com/posts/hexo-website.png) 

### 编写博客

Hexo 博客默认自带一篇教程《Hello World》，告诉用户如何去创建一篇新的文章。创建文章其实也很方便，首先需要在命令行中键入 `hexo new post <title>`，这里 `<title>` 是文章的标题（这里建议使用英文，对 url 比较友好，实际显示的标题可以到 markdown 文件中修改）。

```bash
$ hexo new post "my first article"
INFO  Created: ~/blog-test/source/_posts/my-first-article.md
```

从命令的输出也可以看到，hexo 会在 `<blog_directory>/source/_posts` 文件夹下创建一个 `.md` 的 markdown 文档，文件名则是将标题中的空格替换成 `-` 的结果，例如这里的 `my-first-article.md`。

OK，然后你就可以用任何你喜欢的 markdown 编辑器打开这个文档尽情进行编辑了。markdown 文件里的 `title` 是实际显示的标题，可用和文件名的 title 不一致。另外可用添加 `tags` 和 `categories`，用于制作标签和分类进行索引。

```markdown
title: my first article
date: 2017-09-26 14:34:41
tags: [xiashuo]
categories: xiashuo
---
GNU is Not Unix
<!--more-->

**GNU** is Not Unix is Not Unix is Not Unix is Not Unix is Not Unix is Not Unix...

* GNU
* is
* Not
* Unix

```

> tips: 在编写时，注意使用 `<!--more-->`，hexo 的首页默认是会把整篇文章渲染出来，使用 `<!--more-->` 之后只会显示该标签之上的内容，利用这个标签可用让文章在首页只显示摘要。

另外，这里推荐几个可能的编辑环境和 preview 插件：

* vim + vim-markdown + vim-instant-markdown
* vscode + 内置 Instant Markdown
* sublime + MarkdownEditing + MarkdownPreview
* 各平台下的 markdown 软件，如 MacDown 等

编写完毕后，运行命令 `hexo generate` 或者 `hexo g` 可用生成对应的静态 HTML 文件。

```bash
$ hexo g
INFO  Start processing
INFO  Files loaded in 359 ms
INFO  Generated: index.html
INFO  Generated: archives/index.html
INFO  Generated: fancybox/blank.gif
...
INFO  Generated: css/fonts/fontawesome-webfont.ttf
INFO  Generated: 2017/09/26/hello-world/index.html
INFO  Generated: 2017/09/26/my-first-article/index.html
INFO  31 files generated in 1.75 s
```

使用命令 `hexo server` 或者 `hexo s` 可用在本地调试已经写完的文章（使用 `hexo s` 会自动生成静态文件，因此不必在 `hexo s` 之前运行 `hexo g`）。

```bash
$ hexo s
INFO  Start processing
INFO  Hexo is running at http://0.0.0.0:4000/. Press Ctrl+C to stop.
```

再次登录页面后可以看到已经有已经写完的文章了。

![hexo-article](http://cdn.jetmuffin.com/posts/hexo-article.png)

### 部署到 Github Pages

Hexo 最方便的一点是可以配合 Github Pages 使用。

> Github Pages is Websites for you and your projects. Hosted directly from your GitHub repository. Just edit, push, and your changes are live.

首先登录你的 Github 主页，新建一个名为 `<yourname>.github.io` 的仓库，这里 `<yourname>` 指的是你的 github 用户名，例如笔者用户名为 jetmuffin，则新建一个名为 `jetmuffin.github.io` 的仓库。这个仓库名和未来访问的地址绑定，即之后可以通过 [http://yourname.github.io](#) 来访问你的博客。如果你需要自己绑定一个域名，那么可以不按这个规则来创建。

然后编辑你的 hexo 目录下的 `_config.yml` 文件，笔者的 hexo 目录为 `~/blog`，那么则需要编辑 `~/blog/_config.yml`。在文件的最底部可以看到 `Deployment` 的配置：

```yaml
# Deployment
## Docs: http://hexo.io/docs/deployment.html
deploy:
  type:
```

把它修改为你的 github 对应地址：

```yaml
# Deployment
## Docs: http://hexo.io/docs/deployment.html
deploy:
  type: git
  repo: git@github.com:jetmuffin/jetmuffin.github.io.git
  branch: master
```

这里需要解释一下配置的内容：

* `repo` 对应刚刚创建的仓库的地址（带 .git），切记使用 `ssh` 模式，不然每次 deploy 都要输密码
* `branch` 是指 hexo generate 出来的静态 HTML 存放的分支，这里可以直接使用 `master`，或者选择其他分支，但是需要配合 github pages 里的设置。

修改完毕后，使用命令 `hexo deploy` 或者 `hexo d` 就可以把生成的静态 HTML 部署上 github pages 了。

> tips: `hexo deploy` 只会 diff 静态 HTML 文件，因此如果要 deploy，必须提前执行过 `hexo generate` 把修改或者添加的内容更新到静态的 HTML 上。

```bash
$ hexo deploy
[master 96f3fef] Site updated: 2017-09-26 15:20:43
 43 files changed, 2533 insertions(+), 687 deletions(-)
 create mode 100644 2017/09/26/write-blog-with-hexo-and-aggregate-to-nap-blog/index.html
 ...
remote: This repository moved. Please use the new location:
remote:   git@github.com:JetMuffin/jetmuffin-blog.git
To github.com:JetMuffin/jetmuffin.github.io.git
   3c38954..96f3fef  master -> gh-pages
Branch master set up to track remote branch gh-pages from git@github.com:JetMuffin/jetmuffin.github.io.git.
INFO  Deploy done: git
```

接着打开 [http://yourname.github.io](http://yourname.github.io) 就可以看到文章已经部署上去了。

### 保存 Hexo 源码

除了利用 github 托管博客生成的静态 HTML 文件外，这里建议把 hexo 源码也托管到 github 上。可以使用之前创建的 `<yourname>.github.io` 这个仓库，放在不同的分支上，例如这里我放在 `hexo` 这个 branch 上。

首先在本地 `git init`，添加 `.gitignore`，再添加博客目录下的内容。添加一个 `remote`，指向 `<yourname>.github.io` 仓库。

```bash
$ cd ~/blog
$ git init
Initialized empty Git repository in /Users/jeff/blog-test/.git/
$ git checkout -b hexo
$ echo 'node_modules' >> .gitignore
$ git add .
$ git commit -m "Fisrt commit"
79 files changed, 5414 insertions(+)
 create mode 100644 .gitignore
 create mode 100644 _config.yml
 ...
$ git remote add origin git@github.com:JetMuffin/jetmuffin.github.io.git
$ git push origin hexo
```

> **NOTE:** 一定要把 `node_modles` 添加到 `.gitignore`，不然会有大量的依赖文件被追踪；git push 时需要手动 git checkout 切换到 push 的分支。

另外也可以将源码放在 master 分支，生成的静态页面放在其他的分支（例如 `gh-pages` 分支），这样的话不需要进行分支切换，但是需要手动到项目仓库 `settings/Options/GitHub Pages` 下配置 github page 对应的分支，如下图：

![gh-pages](http://cdn.jetmuffin.com/posts/gh-pages.png)

至此，hexo 博客就搭建完毕了。

另外，hexo 的其他具体使用可以查阅 hexo 对应的文档：

* [hexo 命令介绍](https://hexo.io/zh-cn/docs/commands.html)
* [hexo 添加主题](https://hexo.io/zh-cn/docs/themes.html)
* [hexo 模板介绍](https://hexo.io/zh-cn/docs/templates.html)

## 使用 NAP-blog-webhook

NAP-blog-webhook 是基于开源项目 [NetEaseGame/git-webhook](https://github.com/NetEaseGame/git-webhook) 修改的项目，所以这里先感谢一波原作者。

### 工作原理

首先介绍一下什么是 Git 的 WebHook？以 GitHub 为例，在项目的 Setting 页面，可以看到一栏 WebHook 菜单，可以配置这个项目的 WebHook，也就是可以指定当项目发生某些情况的时候，去 POST 请求一个 Web Url 地址，同时携带当前项目的一些信息。

同样的，任何 Git 服务都有这样的 WebHook 设置。我们利用 Git 的这个机制，可以在 Web Url 这里做拉取代码，自动部署，代码增量检查，CI 等等，同样，包括 Travis-ci 等很多 GitHub 第三方集成都是这样的一个原理。

本项目（NAP-blog-webhook）就是基于这样的原理，做的一个博客聚合,当注册的博客更新后,会通过 webhook 告知 robot, robot 抓取相应修改并聚合到 NAP 组的博客中。

### 如何使用

首先需要注册 webhook，在 Blog 标签页里点击添加新的博客，在 Git Repository 里填写 你的博客仓库名(例如 `jetmuffin-blog`, `konnase.github.io`). 然后在 Branch 一栏填写博客源代码所在分支(即上文提到的 hexo 分支，或有 `source/_posts/xxx.md` 的分支)。最后在 URL 一栏填写博客的访问地址，如下图所示：

![step1](http://cdn.jetmuffin.com/posts/webhook-step1.png)

然后点击 **Operate 里的复制按钮**，将分配到的 webhook 地址拷贝到粘贴板。

![step2](http://cdn.jetmuffin.com/posts/webhook-step2.png)

接着需要到博客对应的仓库，即上文创建的仓库里添加 git webhook，在项目的 `settings/webhooks/Add webhook` 界面中填写信息。其中 `Payload URL` 填写刚刚复制到粘贴板的地址，`Content type` 需要修改为 `application/json`，如下图所示：

![step3](http://cdn.jetmuffin.com/posts/webhook-step3.png)

然后每当更新一篇文章后，除了用 `hexo deploy` 部署到 github-pages 外，还需要把源码提交到源码仓库，以触发 webhook。需要在 git commit 提交时，加入关键字 **robot**，来互换机器人爬取这篇文章（只要 commit 信息中出现 **robot** 这个单词即可）。

> **NOTE:** 必须是源码进行提交才会被爬取，hexo deploy 生成的静态文件提交不会被爬取。

![step4](http://cdn.jetmuffin.com/posts/webhook-step4.png)

接着在 NAP-blog-webhook 的 [Dashboard](http://webhook.njuics.cn/#/blog) 里可以看到触发的 webhook，点击博客名可以看到 webhook 触发历史和爬取日志。

![step5](http://cdn.jetmuffin.com/posts/webhook-step5.png)

最后，爬取的文章会被汇聚到 Gitlab 的仓库中，并且以 Merge Request 的形式等待审阅。

如果你是仓库的管理员，那么你可以自己 review 这个 MR，如果保证没有错误，便可以 Merge 这个 MR。Merge 操作会触发 Continuous Delivery，自动更新文章到 NAP-blog 上。如果你不是管理员，那么需要喊一个管理员来帮你 review 这个 MR。

![step6](http://cdn.jetmuffin.com/posts/webhook-step6.png)

### Q&A

#### 1. 为什么我的提交被 Skipped 了？

目前 NAP-blog-webhook 只会爬取 commit 信息中修改过的 `source/_posts` 路径下的 `*.md` 文件，其余会被跳过，因此你需要检查一下你的提交是否正确。

#### 2. 我提交了一个错误的 commit 没有抓取文章怎么办？

没关系，到这篇文章里，随便找个地方加个空格或者换行什么的，这样就有 diff 了，再重新单独 commit 这篇文章，再次触发 webhook 即可。

#### 3. 我的提交是正确的，但是抓取出错了怎么办？

在 Dashboard 中，把鼠标放在 status 的 `成功/失败/异常/未知/执行` 状态标签上，会出现一个 retry 的按钮，点击按钮可以手动重新触发 webhook。如果还有错误，那么很抱歉，代码里有 BUG，请联系 [JetMuffin](mailto:564936642@qq.com) [手动笑哭.png]。