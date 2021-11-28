# 开发环境
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md)

## 工具

### [必需工具]
+ [Visual Studio](https://visualstudio.microsoft.com)
+ [CMake](https://cmake.org/)
+ [vcpkg](https://github.com/microsoft/vcpkg/releases)
+ [上古卷轴5: 天际特别版 1.5.97](https://store.steampowered.com/app/489830/The_Elder_Scrolls_V_Skyrim_Special_Edition)

本教程会使用CMake管理插件项目，并用Visual Studio 2022进行开发和编译.

### [可选工具]
+ [Steamless (用于去除Steam的反Debug保护)](https://github.com/atom0s/Steamless)
+ [ReClass.NET (用于分析内存结构, 反编译)](https://github.com/ReClassNET/ReClass.NET)
+ [x64DBG (用于分析指令集, 设置断点, 生成签名)](https://x64dbg.com/#start)
+ [CheatEngine (用于分析内存地址, 寻找指针)](https://www.cheatengine.org)

> Steamless使用方法: 到`Release`页面下载最新版Steamless, 解压缩后运行`Steamless.exe`, 选择`SkyrimSE.exe`并点击运行即可移除Steam的DRM验证.

## 配置
### [CMake]
使用`CMake`保持外部编译的规范，有助于管理插件项目和依赖项. [下载CMake](https://cmake.org/)后安装. 

### [vcpkg]
`vcpkg`用于获取和更新依赖项. [下载vcpkg](https://github.com/microsoft/vcpkg/releases)后解压至合适位置.  
在环境变量-系统变量中添加变量`VCPKG_ROOT`并为其赋值本地`vcpkg`的安装目录.  
![vcpkgAddEnv](/images/env_var.png)

### [MO2] 
在MO2新建一个档案(复制已有档案)用于调试插件.  
![MO2AddProfile](/images/mo2_addprof.png)  
在环境变量-系统变量中添加变量`MO2Path`并为其赋值天际特别版MO2的数据目录. 

### [游戏配置]
确认游戏本体可以正常运行;  
确认游戏可以从SKSE正常运行;
确认游戏可以从MO2正常运行;  
在环境变量-系统变量中添加变量`Skyrim64Path`并为其赋值天际特别版的安装目录.

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
其中`iSize H`和`iSize W`值无需设置为1366x768, 这里只为使用一个比全屏幕分辨率低的窗口模式, 方便切出游戏和调试.

### [示例项目]
[下载至本地](https://github.com/gottyduke/PluginTutorialCN/archive/refs/heads/master.zip)后解压`example`文件夹至合适位置, 这是教程的示例项目, 包含一个简单易用的`powershell`脚本辅助开发. 

### [依赖项]
本教程示例项目依赖于`spdlog`, `CommonLibSSE`, 以及`DKUtil`.
打开`powershell`或常用的命令行终端, 重定向至`example`示例项目内. 运行以下命令:
```powershell
pushd
cd $ENV:VCPKG_ROOT
.\vcpkg install spdlog:x64-windows-static-md
.\vcpkg install boost-stl-interfaces:x64-windows-static-md
popd
mkdir extern
cd .\extern
git clone https://github.com/Ryan-rsm-McKenzie/CommonLibSSE
git clone https://github.com/gottyduke/DKUtil
cd ..
cmake -B build -S .
```
完成后打开`build\Template.sln`解决方案并编译. 编译后的二进制文件将会自动拷贝至MO2目录(MO2界面内按F5刷新).
> `ZERO_CHECK`用于在VS内同步CMakeLists.txt的更新.
***
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [探索未知](/docs/ToUnknown.md)