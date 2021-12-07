<h1 align="center">工具配置</h1>
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>

---
<h2 align="center">前置</h2>

+ ### 必需
    + [CMake](https://cmake.org)
    + [Git](https://git-scm.com)
    + [vcpkg](https://github.com/microsoft/vcpkg/releases)
    + [Visual Studio](https://visualstudio.microsoft.com)
    + [上古卷轴5: 天际特别版 1.5.97](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)
    + [上古卷轴5: 天际年度版 1.6.xxx](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)

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
本教程使用的IDE为VS2022. [下载Microsoft Visual Studio 2019或2022预览版](https://visualstudio.microsoft.com)后依说明安装.  
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

> [MaxSu的快速入门指南](/docs/QuickStart.md)

+ ### BOOTSTRAP
**首次**设立工作项目需要以**管理员权限**打开`PowerShell`, 重定向至合适的工作目录, 运行以下命令:  
```powershell
git clone https://github.com/gottyduke/SKSEPlugins
Set-ExecutionPolicy Bypass -Scope Process
cd .\SKSEPlugins
.\!Rebuild BOOTSTRAP
```  
在此过程中根据提示完善工程项目相关信息.  
运行完成后**重启**`PowerShell`或常用的命令行终端.  
> `Bypass`执行策略仅应用于当前终端, 不必担心电脑安全.  

+ ### 生成方案
命令`!Rebuild <编译库> <游戏版本> [自定义CLib]`用于重新生成整个解决方案.  
打开`PowerShell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!Rebuild MT AE
```
这样便生成了一个解决方案, 该方案的编译模式为静态编译`MT`, 编译目标为天际年度版`AE`, 使用的`CommonLib`为[默认CLib](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE).  
![CLI_Build](/images/setup/rebuilt.png)  

+ ### 新建项目
命令`!MakeNew <项目名称> [-install: mod名称] [-message: 项目说明] [-vcpkg: 额外依赖项]`用于快速新建符合此工作项目规格的插件项目.
打开`PowerShell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!MakeNew MyNewPlugin -install "My New Mod" -message "Demo Plugin :)" -vcpkg boost-stl-interfaces, spdlog
```
这样便新建了一个插件项目`MyNewPlugin`, 该项目的mod名称为`My New Mod`, 简易说明为`Demo Plugin :)`, 额外的依赖项为`boost-stl-interfaces`和`spdlog`.  
> `!MakeNew`命令生成的插件项目非常基础, 需要根据插件作者自身需求对`CMakeLists.txt`和`vcpkg.json`进行进一步的修改.  

+ ### 添加/删除文件
在插件项目内`src`文件夹内新建文件/复制文件后, 或者在`include`文件夹内添加外部文件后, 打开`PowerShell`或常用的命令行终端, 重定向`SKSEPlugins`目录内, 运行`.\!Rebuild`(无参数)并在VS内编译项目`ZERO_CHECK`. 更新项目文件完成后根据提示重新加载解决方案即可使用新添加的文件.  
> 不要在VS内手动添加文件.  
> `ZERO_CHECK`用于在VS内同步应用于`CMakeLists.txt`的更新.  
> 使用`CMake`管理插件项目初期可能会使习惯于直接编译项目的插件作者感到很不适应, 但外部编译(out-of-source build)将开发编译期产生的繁杂文件和纯净的插件项目源文件分离, 有助于后期的管理和拓展.  
> `!Update`脚本目前支持自动添加至CMake源文件表的文件类别为: `*.c` `*.cpp` `*.cxx` `*.h` `*.hpp` `*.hxx`

+ ### 自定义CLib
`!Rebuild`命令生成时使用[默认CommonLib](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE). 若要使用自定义CommonLib, 在`BOOTSTRAP`步骤中设置合适的自定义CommonLib环境并在`!Rebuild`命令后附加数字参数`0`以启用自定义CommonLib. 
> 使用自定义CommonLib时, `!Rebuild`命令默认该自定义CommonLib符合当前编译目标(`AE`或`SE`)

---
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>
