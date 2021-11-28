# 开发环境
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md)

## 工具

### [必需工具]
+ [Visual Studio](https://visualstudio.microsoft.com)
+ [CMake](https://cmake.org/)
+ [vcpkg](https://github.com/microsoft/vcpkg/releases)
+ [上古卷轴5: 天际特别版 1.5.97](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)
+ [上古卷轴5: 天际年度版 1.6.xxx](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)

本教程会使用CMake管理插件项目，并用Visual Studio 2022进行开发和编译.

### [可选工具]
+ [Steamless (用于去除Steam的反Debug保护)](https://github.com/atom0s/Steamless)
+ [ReClass.NET (用于分析内存结构, 反编译)](https://github.com/ReClassNET/ReClass.NET)
+ [x64DBG (用于分析指令集, 设置断点, 生成签名)](https://x64dbg.com/#start)
+ [CheatEngine (用于分析内存地址, 寻找指针)](https://www.cheatengine.org)

> Steamless使用方法: 到`Release`页面下载最新版Steamless, 解压缩后运行`Steamless.exe`, 选择`SkyrimSE.exe`并点击运行即可移除Steam的反Debug保护.

## 配置
### [CMake]
使用`CMake`保持外部编译的规范，有助于管理插件项目和依赖项. [下载CMake](https://cmake.org/)后依说明安装.  

### [vcpkg]
`vcpkg`用于获取和更新依赖项. [下载vcpkg](https://github.com/microsoft/vcpkg/releases)后依说明安装.  
在环境变量-系统变量中添加变量`VCPKG_ROOT`并为其赋值本地`vcpkg`的安装目录.  
![vcpkgAddEnv](/images/env_var.png)

### [MO2] 
在MO2新建一个档案(复制已有档案)用于调试插件.  
![MO2AddProfile](/images/mo2_addprof.png)  

确认此MO2配置的路径为`游戏目录/MO2`.  
![MO2Base](/images/mo2_base.png)  

如果需要同时保留天际特别版和天际年度版平行开发环境, 确认使用两份不同的MO2配置.  
![MO2Parra](/images/mo2_parra.png)  


### [游戏配置]
确认游戏本体可以正常运行;  
确认游戏可以从SKSE正常运行;
确认游戏可以从MO2正常运行;  
在环境变量-系统变量中添加变量`SkyrimSEPath`并为其赋值天际特别版的安装目录.  
如果需要同时保留天际特别版和天际年度版平行开发环境, 在环境变量-系统变量中添加变量`SkyrimAEPath`并为其赋值天际年度版的安装目录, 并确认以上适用于平行版本.

移除/禁用ENB和ReShade插件(会减缓启动速度, 妨碍调试);

使用BethINI工具或者手动配置游戏`SkyrimPrefs.ini`文件(同步至MO2):
```
[Display]
bBorderLess=0
bFullScreen=0
iSize H=768
iSize W=1366
iVSyncPresentInterval=1
```
其中分辨率无需设置为1366x768, 这里只为使用一个比全屏幕分辨率低的窗口模式, 方便切出游戏和调试.

### [示例项目]
因为作者特别懒所以直接用自己的工作项目做示例了.
打开`powershell`或常用的命令行终端, 重定向至合适的工作目录. 运行以下命令:  
```powershell
git clone --recurse-submodules https://github.com/gottyduke/SKSEPlugins
cd .\SKSEPlugins
& $env:VCPKG_ROOT\vcpkg install boost-stl-interfaces:x64-windows-static
& $env:VCPKG_ROOT\vcpkg install rsm-binary-io:x64-windows-static
& $env:VCPKG_ROOT\vcpkg install spdlog:x64-windows-static
& $env:VCPKG_ROOT\vcpkg install xbyak:x64-windows-static
.\!Rebuild MT
```  
在环境变量-系统变量中添加变量`CommonLibSSEPath`并为其赋值`CommonLibSSE`的存放目录(`路径/SKSEPlugins/Library/CommonLibSSE`).  
在环境变量-系统变量中添加变量`DKUtilPath`并为其赋值`DKUtil`的存放目录(`路径/SKSEPlugins/Library/DKUtil`).  

### [脚本说明]
作者的工作项目包含三个常用脚本辅助开发, `!Rebuild`, `!Update`, 和`!MakeNew`.  

`!Rebuild [编译库:MT|MD]`用于重新生成整个解决方案. 参数`MD`为动态编译`MultiThreadedDLL`, 使用的`vcpkg`为`x64-windows-static-md`. 如无特殊需求, 建议使用参数`MT`来生成静态编译`MultiThreaded`, 使用的`vcpkg`为`x64-windows-static`. 无参数运行时会刷新`CMakeLists.txt`以重新更新VS解决方案(下方解释).  

`!MakeNew <项目名称> <项目类别:P|L>`用于快速新建符合此工作项目规格的插件项目(`P`)或库项目(`L`).  

`!Update <运行模式:COPY|SOURCEGEN|DISTRIBUTE>`用于编译后复制文件, 为`CMakeLists.txt`生成源文件表, 以及自动更新. 在正常使用此工作项目时, 此脚本会被自动更新到每一个下属项目中并嵌入至生成的解决方案中, 不需要单独运行.  

### [新建项目]
命令`!MakeNew <项目名称> <项目类别:P|L>`可以用于快速新建插件项目或库项目.
打开`powershell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
cd .\SKSEPlugins
.\!MakeNew MyNewPlugin P
.\!Rebuild MT
```
完成后打开`Build\skse64.sln`解决方案并编译`MyNewPlugin`项目. 编译后的二进制文件将会自动拷贝至MO2目录(MO2界面内按F5刷新).  
> `!MakeNew`命令生成的插件项目非常基础, 需要根据插件作者自身需求对`CMakeLists.txt`和`vcpkg.json`进行进一步的修改.  

#### [添加文件]
在插件项目内`src`文件夹内新建文件/复制文件后, 或者在`include`文件夹内添加外部包含库文件后, 在`SKSEPlugins`目录下运行`.\!Rebuild`(无参数)并在VS内编译项目`ZERO_CHECK`. 更新项目文件完成后重新加载解决方案即可使用新添加的文件.    
> 不要在VS内手动添加文件.  
> `ZERO_CHECK`用于在VS内同步应用于`CMakeLists.txt`的更新.  
> 使用`CMake`管理插件项目初期可能会使习惯于直接编译项目的插件作者感到很不适应, 但外部编译(out-of-source build)将开发编译期产生的繁杂文件和纯净的插件项目源文件分离, 有助于后期的管理和拓展.

***
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md)