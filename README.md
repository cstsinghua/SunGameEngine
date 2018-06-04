# SunGameEngine
A demo javascript engine demonstrates how to build a javascript engine on Android platform based on [v8](https://github.com/v8/v8) and [openGL ES](https://www.khronos.org/opengles/).

1.git clone https://github.com/cstsinghua/SunGameEngine.git;

2.enter app/src/main/cpp/static_lib directory,unzip v8_arm_arm64.zip,then copy all subdirs(arm64-v8a, armeabi-v7a, include, v8_src_include) to app/src/main/cpp directory(overwrite the old files);

3.open Android studio(version 3.1 is recommended),open and load this project;

4.run it and view the result.

# 前言
本文旨在介绍如何开发一个用于Android平台的JS game engine。即可以通过JS编写游戏UI和业务逻辑代码。与业界已有的[Cocos2d-JS](http://www.cocos.com/docs/js/1-about-cocos2d-js/1-1-a-brief-history/zh.html)引擎的原理类似。但本引擎基于OpenGL和google的[v8 js engine](https://github.com/v8/v8)。

# 思路
先放一个参考开源库：https://github.com/BonsaiDen/Fluff.js

该库的思路大致如下：
1. 初始化的时候，进入C++的代码的main函数(在fluff.cc里面)，在里面执行runGame函数：
	![](https://i.imgur.com/pBoP235.png)
2. 在initFluff函数内部，将封装OpenGL的C++函数通过v8接口暴露给JS，也就是JS脚本里面就可以直接使用暴露出来的接口，相当于在JS里面可以直接require或者import这些类和函数，直接调用即可(类似libtolua.so和device插件里面使用到的依赖库[lua-intf](https://github.com/SteveKChiu/lua-intf)干的事)；
3. executeScript在uitl.cc里面，就是加载JS的代码入口，界面UI和业务逻辑用js编写，在js文件里面，这将入口的js脚本加载，然后执行js的入口函数(callFunction去执行onload)。对比一下就是，当前公司引擎libbabe.so，加载之后会去加载main.lua并执行main.lua的入口函数，从而就把整个流程串起来了。

**Note：Fluff库是2010年编写的，当时的v8的版本比较老了，现在v8的API有较大变动，因此留意。此外，该库使用的是openGL 1.0，而当前PC端主流的版本是openGL 3.x和4.x，移动平台主流版本是openGL ES 2.0和openGL ES 3.x**

对比而言，我们将v8的so(关于v8的动态库或静态库构建，请参见[v8 engine构建指南](https://cstsinghua.github.io/2018/04/04/%E6%9E%84%E5%BB%BAv8%20engine%E6%8C%87%E5%8D%97))和OpenGL的接口封装链接到一个我们自创的JS Android game engine（取名sun），最终叫libsun.so，这里libsun.so在Activity的onCreate或者间接的其他java类里面(`System.loadLibrary("sun");`)。JNI入口取名initSun，在对应的C++方法里面，初始化v8，将封装的OpenGL绘图接口暴露给JS；然后编写js代码，放置在assets目录的script子目录下，取名main.js,入口函数function onDraw()， 在js里面调用暴露出来的绘图接口(比如draw)绘制，比如绘制三角形，矩形等等，这样就是一个小麻雀JS引擎(功能虽小，但是五脏俱全，核心思路就是这样，Cocos2DJS引擎肯定也是这样干的)

# 开发步骤
待续

# 成果
本人写了一个简单的demo JS engine，请在Github上查看[SunGameEngine](https://github.com/cstsinghua/SunGameEngine)



