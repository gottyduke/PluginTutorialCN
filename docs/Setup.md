# 开发环境
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md) | [快速入门](/docs/QuickStart.md)

## 前置

### 必需工具
+ [CMake](https://cmake.org)
+ [Git](https://git-scm.com)
+ [vcpkg](https://github.com/microsoft/vcpkg/releases)
+ [Visual Studio](https://visualstudio.microsoft.com)
+ [上古卷轴5: 天际特别版 1.5.97](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)
+ [上古卷轴5: 天际年度版 1.6.xxx](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)

本教程会使用CMake管理插件项目，并用Visual Studio 2022进行开发和编译.

### 可选工具
+ [Steamless (用于去除Steam的反Debug保护)](https://github.com/atom0s/Steamless)
+ [ReClass.NET (用于分析内存结构, 反编译)](https://github.com/ReClassNET/ReClass.NET)
+ [x64DBG (用于分析指令集, 设置断点, 生成签名)](https://x64dbg.com/#start)
+ [CheatEngine (用于分析内存地址, 寻找指针)](https://www.cheatengine.org)

> Steamless使用方法: 到`Release`页面下载最新版Steamless, 解压缩后运行`Steamless.exe`, 选择`SkyrimSE.exe`并点击运行即可移除Steam的反Debug保护.  

---
## 工具配置

### CMake
使用`CMake`保持外部编译的规范, 有助于管理插件项目和解决方案. [下载CMake](https://cmake.org)后依说明安装.  

### Git
`Git`用于拉取/更新库. [下载Git](https://git-scm.com)后依说明安装.

### vcpkg
`vcpkg`用于获取和管理依赖项. 如果本地已安装`vcpkg`, 则在系统环境变量添加变量`VCPKG_ROOT`并为其赋值本地`vcpkg`的安装目录.  
![vcpkgAddEnv](/images/env_var.png)  
如果本地未安装`vcpkg`, 教程示例项目会自动安装`vcpkg`.  

### Visual Studio
本教程使用的IDE为VS2022. [下载Microsoft Visual Studio 2019或2022预览版](https://visualstudio.microsoft.com)后依说明安装.  
必需组件: `使用C++的桌面开发`  
![VSCXX](/images/vscxx.png)  

---
## 游戏配置

确认游戏本体可以正常运行;  
确认游戏可以从SKSE正常运行;  
移除/禁用ENB和ReShade插件(会减缓启动速度, 妨碍调试);  

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
## 示例项目

### BOOTSTRAP
**第一次运行**示例项目需要以**管理员权限**打开`powershell`或常用的命令行终端, 重定向至合适的工作目录, 运行以下命令:  
```powershell
Set-ExecutionPolicy -ExecutionPolicy   -Scope LocalMachine
git clone https://github.com/gottyduke/SKSEPlugins
cd .\SKSEPlugins
.\!Rebuild BOOTSTRAP
```  
此过程中根据提示完善工程项目相关信息.  
运行完成后**重启**`powershell`或常用的命令行终端.  

### 生成方案
命令`!Rebuild <编译库> <游戏版本> [自定义CLib]`用于重新生成整个解决方案.  
打开`powershell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!Rebuild MT AE
```
这样便生成了一个解决方案, 该方案的编译模式为静态编译`MT`, 编译目标为天际年度版`AE`, 使用的`CommonLib`为最新版.  
![CLI_Build](/images/rebuilt.png)  

### 新建项目
命令`!MakeNew <项目名称> [mod名称] [项目说明] [额外依赖项]`用于快速新建符合此工作项目规格的插件项目.
打开`powershell`或常用的命令行终端, 重定向`SKSEPlugins`目录内. 运行以下命令:  
```powershell
.\!MakeNew MyNewPlugin -install "My New Mod" -message "Demo Plugin :)" -vcpkg boost-stl-interfaces, spdlog
```
这样便新建了一个插件项目`MyNewPlugin`, 该项目的mod名称为`My New Mod`, 简易说明为`Demo Plugin :)`, 额外的依赖项为`boost-stl-interfaces`和`spdlog`.  
> `!MakeNew`命令生成的插件项目非常基础, 需要根据插件作者自身需求对`CMakeLists.txt`和`vcpkg.json`进行进一步的修改.  
> 版本更迭时, 请勿直接修改任何源文件. 在插件项目目录下`CMakeLists.txt`内修改版本即可.  

### 添加文件
在插件项目内`src`文件夹内新建文件/复制文件后, 或者在`include`文件夹内添加外部包含库文件后, 打开`powershell`或常用的命令行终端, 重定向`SKSEPlugins`目录内, 运行`.\!Rebuild`(无参数)并在VS内编译项目`ZERO_CHECK`. 更新项目文件完成后根据提示重新加载解决方案即可使用新添加的文件.  
> 不要在VS内手动添加文件.  
> `ZERO_CHECK`用于在VS内同步应用于`CMakeLists.txt`的更新.  
> 使用`CMake`管理插件项目初期可能会使习惯于直接编译项目的插件作者感到很不适应, 但外部编译(out-of-source build)将开发编译期产生的繁杂文件和纯净的插件项目源文件分离, 有助于后期的管理和拓展.  

---
## 脚本说明
作者的工作项目包含三个常用脚本辅助开发, `!Rebuild`, `!MakeNew`, 和`!Update`.  

`!Rebuild <编译库|BOOTSTRAP> <游戏版本> [自定义CLib]`用于重新生成整个解决方案.  
参数 | 说明
--- | ---
`BOOTSTRAP` | 设置相应的系统环境变量以及工具链.
`编译库` | `MT`(静态编译`MultiThreaded`)或`MD`(动态编译`MultiThreadedDLL`), 使用的`vcpkg`为`x64-windows-static`或`x64-windows-static-md`. 无特殊需求建议使用`MT`参数.
`游戏版本` | `AE`为天际年度版(默认最新版)或`SE`为天际特别版(默认1.5.97).
`自定义CLib` | 可选参数`0`, 启用自定义CLib代替默认的CLib作为开发库. 不使用自定义CLib时无视此选项.  
无参数 | 同步对项目做出的更改(添加/删除文件等). 更新VS项目`ZERO_CHECK`.  

`!MakeNew <项目名称> [mod名称] [项目说明] [额外依赖项]`用于快速新建符合此工作项目规格的插件项目.  
参数 | 说明
--- | ---
`项目名称` | 项目名称, 同步于`CMakeLists.txt`, `vcpkg.json`和解决方案内.
`mod名称` | `-install`或`-i`, 指定MO2安装时的mod名称(默认为项目名称).
`项目说明` | `-message`或`-m`, 简易的项目说明, 生成于附属的`vcpkg.json`文件内. 用双引号`"`包含.
`额外依赖项` | `-vcpkg`或`-v`, 额外的`vcpkg`依赖项. 默认为`spdlog`. 多个依赖项用逗号分隔. 

`!Update <运行模式>`用于编译后复制文件, 为`CMakeLists.txt`生成源文件表, 以及自动更新. 在正常使用此工作项目时, 此脚本会被自动更新到每一个下属项目中并嵌入至生成的解决方案中, 不需要单独运行.  

***
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md) | [快速入门](/docs/QuickStart.md)