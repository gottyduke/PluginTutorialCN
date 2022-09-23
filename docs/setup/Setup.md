<h1 align="center">工具配置</h1>
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

<h2 align="center">前置</h2>

+ ### 必需
    + [CMake](https://cmake.org)
    + [Git](https://git-scm.com)
    + [vcpkg](https://github.com/microsoft/vcpkg/releases)
    + [Visual Studio 17 2022](https://visualstudio.microsoft.com)
    + [上古卷轴5: 天际特别版 SE 1.5.3 ~ 1.5.97](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)
    + [上古卷轴5: 天际周年版 AE 1.6.317+](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)

本教程会使用CMake管理插件项目，并用Visual Studio 2022进行开发和编译.

+ ### 可选
    + [Steamless (用于去除Steam的反Debug保护)](https://github.com/atom0s/Steamless)
    + [ReClass.NET (用于分析内存结构, 反编译)](https://github.com/ReClassNET/ReClass.NET)
    + [x64DBG (用于分析指令, 设置断点, 生成签名)](https://x64dbg.com/#start)
    + [CheatEngine (用于分析内存地址, 寻找指针)](https://www.cheatengine.org)

> Steamless使用方法: 运行`Steamless.exe`并解包`SkyrimSE.exe`. 运行完成后将`SkyrimSE.exe`更名为`SkyrimSE.Old.exe`, 并将`SkyrimSE.unpacked.exe`更名为`SkyrimSE.exe`即可解除Steam的反Debug保护.  

---
<h2 align="center">工具</h2>

+ ### CMake
使用`CMake`保持外部编译的规范, 有助于管理插件项目和解决方案. [下载CMake](https://cmake.org)后依说明安装.  

+ ### Git
`Git`用于拉取/更新库. [下载Git](https://git-scm.com)后依说明安装.

+ ### vcpkg
`vcpkg`用于获取和管理依赖项. 如果本地已安装`vcpkg`, 则在系统环境变量添加变量`VCPKG_ROOT`并为其赋值本地`vcpkg`的安装目录.  
如果本地未安装`vcpkg`, 教程示例项目会自动安装`vcpkg`.  

+ ### Visual Studio
本教程使用的IDE为VS2022. [下载Microsoft Visual Studio 2022预览版](https://visualstudio.microsoft.com)后依说明安装.  
必需组件: `使用C++的桌面开发`  
![VSCXX](/images/setup/vscxx.png)  

---
<h2 align="center">游戏</h2>

+ 确认游戏本体可以正常运行;  
+ 确认游戏可以从SKSE正常运行;  
+ 移除/禁用ENB和ReShade插件(会减缓启动速度, 妨碍调试);  

> 可选: 使用BethINI工具或者手动配置游戏`SkyrimPrefs.ini`文件:
> ```
> [Display]
> bBorderLess=0
> bFullScreen=0
> iSize H=768
> iSize W=1366
> iVSyncPresentInterval=1
> ```
> 其中分辨率无需设置为`1366x768`, 这里只为使用一个比全屏幕分辨率低的窗口模式, 方便切出游戏和调试.

---
<h2 align="center">示例项目</h2>

[Maxsu的快速入门指南(未更新)](/docs/QuickStart.md)


+ ### CommonLibSSE
简称`CLib`, `CommonLib`, 是代替SKSE的插件开发库, 内容极为丰富, 实用性高, 并且社区一直进行维护和更新.  
当前社区主流的有Ryan开发的`CommonLibSSE`(原作者), po3更新的`CommonLibSSE`(维护者), maxsu更新的`CommonLibSSE`(维护者).  
除Ryan的`CLib`库外, 其他提到的库都支持`NG`构建. `NG`则是`Next-Generation`的缩写, 由CharmedBaryon在po3的`CLib`库基础上升级出的可同时支持AE/SE/VR版的多目标运行环境.  


+ ### BOOTSTRAP
**首次**设立工作项目需要以**管理员权限**打开`PowerShell`, 重定向至合适的工作目录, 运行以下命令:  
```powershell
git clone https://github.com/gottyduke/SKSEPlugins
Set-ExecutionPolicy Bypass -Scope Process
cd .\SKSEPlugins
.\!Rebuild -Bootstrap
```  
![win_terminal.png](/images/setup/win_terminal.png)  
在此过程中根据提示完善工程项目相关信息.  
运行完成后**重启**`PowerShell`或常用的命令行终端.  
> `Bypass`执行策略仅应用于当前终端, 不必担心电脑安全.  


+ ### 新建项目
命令`!MakeNew <项目名称> [-install: mod名称] [-message: 项目说明] [-vcpkg: 额外依赖项]`用于快速新建符合此工作项目规格的插件项目.
打开`PowerShell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!MakeNew MyNewPlugin -install "My New Mod" -message "Demo Plugin :)"
```
这样便新建了一个插件项目`MyNewPlugin`, 该项目的mod名称为`My New Mod`, 简易说明为`Demo Plugin :)`.  
> `!MakeNew`命令生成的插件项目非常基础, 若有第三方库依赖, 需要对`CMakeLists.txt`和`vcpkg.json`进行进一步的修改.  
> 插件项目名称指整个项目及生成的dll文件的名称. mod名称指使用生成后控制工具时生成的mod名字.


+ ### 生成方案
命令`!Rebuild <运行环境> [自定义CLib]`用于重新生成整个解决方案.  
打开`PowerShell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!Rebuild All
```
这样便生成了一个解决方案, 该方案的运行环境支持AE版+SE版+VR版, 使用的`CommonLib`为[CharmedBaryon的默认CLib-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG).  

生成一个全版本支持+自定义`CLib`的解决方案, 同时启用`DKUtil`的测试工具:  
`.\!Rebuild -C -DBG DKUtil`  
[![Report.png](https://i.postimg.cc/rpmByPWv/Report.png)](https://postimg.cc/rDBnQgfJ)


+ ### 添加/删除文件  
在VS内正常右键新建文件即可, 新文件将会立即可用. 第一次编译后检测到的新文件会自动转移至该项目的根目录, 并在第二次编译后自动更新项目文件, 此时按照VS提示重新加载项目即可. 这两次编译并不需要立即接连执行, 对文件做出的所有更改在转移前, 两次编译之间, 编译后, 重新加载后都会保留.  
> `!Update`脚本目前支持自动添加至CMake源文件表的文件类别为: `*.c` `*.cpp` `*.cxx` `*.h` `*.hpp` `*.hxx`
![QuickAdd](/images/setup/quick_add.png)


+ ### 运行环境代码分离
使用`REL::Module::IsAE() ::IsSE() ::IsVR()`来判断当前加载插件的运行环境是哪一个版本.  
```C++
if (REL::Module::IsAE()) {
    // AE code
} else if (REL::Module::IsSE()) {
    // SE code
} else {
    // VR code
}
```

+ ### 自定义CLib
`!Rebuild`命令生成时使用[CharmedBaryon的默认CLib-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG). 若要使用自定义CommonLib, 在`BOOTSTRAP`步骤中设置合适的自定义CommonLib环境并在`!Rebuild`命令启用参数`-C`或`-Custom`以启用自定义CommonLib.  

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
