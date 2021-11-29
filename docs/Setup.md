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
在环境变量-系统变量中添加变量`SkyrimAEPath`并为其赋值天际年度版的安装目录.  
如果需要同时保留天际特别版和天际年度版平行开发环境, 在环境变量-系统变量中添加变量`SkyrimSEPath`并为其赋值天际特别版的安装目录, 并确认以上适用于平行版本.

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
以管理员权限启动Windows Terminal或PowerShell, 运行命令更改执行策略:  
```powershell
Set-ExecutionPolicy -ExecutionPolicy AllSigned -Scope LocalMachine
```   
重新打开`powershell`或常用的命令行终端, 重定向至合适的工作目录. 运行以下命令:  
```powershell
git clone --recurse-submodules https://github.com/gottyduke/SKSEPlugins
cd .\SKSEPlugins
& $env:VCPKG_ROOT\vcpkg install boost-stl-interfaces:x64-windows-static
& $env:VCPKG_ROOT\vcpkg install rsm-binary-io:x64-windows-static
& $env:VCPKG_ROOT\vcpkg install spdlog:x64-windows-static
& $env:VCPKG_ROOT\vcpkg install xbyak:x64-windows-static
```  
在环境变量-系统变量中添加变量`CommonLibSSEPath`并为其赋值`CommonLibSSE`的存放目录(`路径/SKSEPlugins/Library/CommonLibSSE`).  
在环境变量-系统变量中添加变量`DKUtilPath`并为其赋值`DKUtil`的存放目录(`路径/SKSEPlugins/Library/DKUtil`).  

### [新建项目]
命令`!MakeNew <项目名称> <项目类别:P|L> [安装名称] [项目说明]`可以用于快速新建插件项目或库项目.
打开`powershell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!MakeNew MyNewPlugin P
.\!Rebuild MT AE
```
![CLI_Build](/images/cli_build.png)  
完成后打开`Build\skse64.sln`解决方案并编译`MyNewPlugin`项目. 编译后的二进制文件将会自动拷贝至MO2目录(MO2界面内按F5刷新).  
![MO2_Built](/images/mo2_built.png)  
> `!MakeNew`命令生成的插件项目非常基础, 需要根据插件作者自身需求对`CMakeLists.txt`和`vcpkg.json`进行进一步的修改.  
> 插件项目内`main.cpp`文件包含默认作者名`Dropckicker`, 需要自行修改.  
> 版本更迭时, 请勿直接修改任何源文件. 在插件项目目录下`CMakeLists.txt`修改版本即可.  

#### [添加文件]
在插件项目内`src`文件夹内新建文件/复制文件后, 或者在`include`文件夹内添加外部包含库文件后, 打开`powershell`或常用的命令行终端, 重定向`SKSEPlugins`目录内, 运行`.\!Rebuild`(无参数)并在VS内编译项目`ZERO_CHECK`. 更新项目文件完成后根据提示重新加载解决方案即可使用新添加的文件.    
> 不要在VS内手动添加文件.  
> `ZERO_CHECK`用于在VS内同步应用于`CMakeLists.txt`的更新.  
> 使用`CMake`管理插件项目初期可能会使习惯于直接编译项目的插件作者感到很不适应, 但外部编译(out-of-source build)将开发编译期产生的繁杂文件和纯净的插件项目源文件分离, 有助于后期的管理和拓展.  

### [脚本说明]
作者的工作项目包含三个常用脚本辅助开发, `!Rebuild`, `!Update`, 和`!MakeNew`.  

`!Rebuild <编译库:MD|MT> <游戏版本:AE|SE> [自定义Clib:路径|0]`用于重新生成整个解决方案.  
参数 | 说明
--- | ---
`MD` `MT` | 动态编译`MultiThreadedDLL`或静态编译`MultiThreaded`, 使用的`vcpkg`为`x64-windows-static-md`或`x64-windows-static`. 无特殊需求建议使用`MT`参数.
`AE` `SE` | 用于编译的游戏版本. `AE`为天际年度版(默认最新版), `SE`为天际特别版(默认1.5.97).
`自定义Clib` | 本地的自定义Clib路径, 或将环境变量`CommonLibSSEPath`改为本地自定义Clib路径, 并在此处用数字`0`代替. 不使用自定义Clib时无视此选项.  
无参数运行时会刷新`CMakeLists.txt`用于同步对项目做出的更改(添加/删除文件等).  

`!MakeNew <项目名称> <项目类别:P|L> [安装名称] [项目说明] [额外依赖项]`用于快速新建符合此工作项目规格的插件项目.  
参数 | 说明
--- | ---
`项目名称` | 项目名称, 同步于`CMakeLists.txt`, `vcpkg.json`和解决方案内.
`P` `L` | 插件项目(`P`)或库项目(`L`).
`安装名称` | 指定MO2安装时的mod名称(默认为项目名称).
`项目说明` | 简易的项目说明, 生成于附属的`vcpkg.json`文件内. 
`额外依赖项` | 额外的`vcpkg`依赖项.  

`!Update <运行模式:COPY|SOURCEGEN|DISTRIBUTE>`用于编译后复制文件, 为`CMakeLists.txt`生成源文件表, 以及自动更新. 在正常使用此工作项目时, 此脚本会被自动更新到每一个下属项目中并嵌入至生成的解决方案中, 不需要单独运行.  

***
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md)