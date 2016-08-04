title: 弹出框popover点击框外区域关闭弹出框效果
tags: [jquery,popover]
categories: 前端开发
date: 2015-03-04 22:11:51
---

bootstrap用多了，前不久自己写一个popover，弹出很简单，但是让弹出框失去焦点消失时有点问题，记录一下：

原先的想法是获取鼠标点击范围，若鼠标点击在弹出框之外，则绑定事件关闭弹出框，代码如下：

<!--more-->

然而这样非常的麻烦，

`<div class="login-popover">

...

</div>

<div id=black-layer></div>`

之后发现加了一层黑色的遮罩后，弹出框必定在遮罩上方，这样其实只要给黑色遮罩绑定事件即可。

代码：

`// 关闭登录框

var close_login_box= function (){

    $('#black-layer').fadeOut();

    $('#login-pop').fadeOut();

};

// 登录按钮点击事件

$('#login-btn').click(function(){

    $('#black-layer').fadeIn();

    $('#login-pop').fadeIn();

    return false;

});

// 点击黑色区域直接关闭登陆框

$('#black-layer').click(close_login_box);`