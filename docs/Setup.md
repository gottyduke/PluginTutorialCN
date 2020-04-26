# 开发环境
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [示例插件](/docs/PluginTemplate.md) | [常用方法](/docs/CommonMethods.md) | [版本集成](/docs/AddressLibrary.md) | [探索未知](/docs/ToUnknown.md) | [CommonLibSSE](/docs/CommonLibSSE.md)

## 工具

### [必需工具]
+ [Visual Studio 2015+](https://visualstudio.microsoft.com)
+ [上古卷轴5: 天际特别版](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)

使用其他IDE例如CLion, VS Code + CMake也可胜任开发, 但skse64源码分发自Visual Studio 2015版本, 若想有效率的查看/利用已有的代码, 使用Visual Studio 2015+是最好的选择.

### [可选工具]
+ [Steamless (用于去除Steam的反Debug保护)](https://github.com/atom0s/Steamless)
+ [ReClass.NET (用于分析内存结构, 反编译)](https://github.com/ReClassNET/ReClass.NET)
+ [x64DBG (用于分析指令集, 设置断点, 生成签名)](https://x64dbg.com/#start)
+ [CheatEngine (用于分析内存地址, 寻找指针)](https://www.cheatengine.org)

> Steamless使用方法: 到`Release`页面下载最新版Steamless, 解压缩后运行`Steamless.exe`, 选择`SkyrimSE.exe`并点击运行即可移除Steam的DRM验证.

## 配置

### [SKSE64]

首先前往[SKSE官网](http://skse.silverlock.org)下载符合游戏版本的SKSE64(`2.0.XX`), 解压缩后将`skse64_2_00_XX\src`文件夹移动到一个适合工作的路径(教程将会使用`2.0.17`对应游戏版本`1.5.97`).

确定必需工具都安装好后, 打开`src\skse64\skse64.sln`文件. 这就是插件开发的"基地", 其他的项目都会在这里进行管理.

如果提示源代码管理(Source Control), 点击永久移除即可. 根据使用的Visual Studio版本不同, 可能会提示重定向SDK和工具集, 点击升级到最新版即可(Retarget). 这是SKSE组开发SKSE64所用的配置, 与插件开发无关.

加载完毕后, 会看到如下项目结构:
```
-skse64
    -common_vc14
    -skse64
    -skse64_common
    -skse64_loader
    -skse64_loader_common
    -skse64_steam_loader
```
其中`common_vc14`项目在`src\skse64`路径内, 其它项目在`src\skse64\skse64`路径内.

选中所有项目(选中第一个项目, 按住`shift`再点击最后一个项目), 右击打开属性面板(Properties), 左上角`配置`选中`所有配置`, 将选项`生成事件->生成后事件->在生成中使用`改为`否`并应用. 同上, 这是skse组开发skse64所用的配置, 与插件开发无关.

选中`skse64`和`skse64_common`两个项目后右击打开属性面板, 将选项`常规->配置类型`改为`静态库(.lib)`并应用. 这样skse64的项目配置就完成了.  

> 可以移除`skse64_loader`, `skse64_loader_common`和`skse64_steam_loader`三个项目. 这是SKSE64的启动器项目, 与插件开发无关.

### [游戏配置]

确认游戏本体可以正常运行;

移除/禁用ENB和ReShade插件(会减缓启动速度, 妨碍调试);

使用BethINI工具或者手动配置游戏`SkyrimPrefs.ini`文件:
```
[Display]
bBorderLess=0
bFullScreen=0
iSize H=768
iSize W=1366
iVSyncPresentInterval=1
```
其中`iSize H`和`iSize W`值不一定设置为1366x768, 这里只是为了使用一个比全屏幕分辨率低的窗口模式, 方便切出游戏和调试.

> 如果使用MO2:  
> 在MO2新建一个档案(复制已有档案)用于调试插件.  
>![MO2AddProfile](/images/mo2_addprof.png)  
> 使用BethINI时将路径设置到MO2的新档案路径再进行配置.  
>![BethINIRedirect](/images/bini_red.png)

***
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [示例插件](/docs/PluginTemplate.md) | [常用方法](/docs/CommonMethods.md) | [版本集成](/docs/AddressLibrary.md) | [探索未知](/docs/ToUnknown.md) | [CommonLibSSE](/docs/CommonLibSSE.md)