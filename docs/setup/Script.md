<h1 align="center">脚本说明</h1>
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

本教程使用三个常用脚本辅助开发, `!Rebuild`, `!MakeNew`, 和`!Update`.  

---
+ ### `!Rebuild <AE|SE|VR|ALL|PRE-AE|FLATRIM> [-C|-Custom] [-N|-NoBuild][-WhatIf][-DBG][-D]`
|运行环境|描述|
|-|-|
|`AE`|仅支持AE版(1.6.317+)|
|`SE`|S仅支持SE版(1.5.3+)|
|`VR`|仅支持VR版(1.3.64)|
|`ALL`|默认;支持所有版本|
|`PRE-AE`|支持天SE版(1.5.3+)和VR版(1.3.64)|
|`FLATRIM`|支持AE版(1.6.317+)和SE版(1.5.3+)|  

运行环境参数不分大小写. 支持所有版本时(默认选项), 可以省略`ALL`不写.  

|开关|描述|
|-|-|
|`-C\|-Custom`|启用自定义`CLib`支持;禁用默认`CLib`|
|`-N\|-NoBuild`|禁用预编译`CLib`库行为;|
|`-WhatIf`|Do not initiate CMake generator; Preview `CMakeLists.txt` instead|
|`-DBG [Project]`|Toggle debugger build for `Project`, i.e. testing suites|
|`-D[CMAKE_ARGUMENTS]`|Pass additional arguments to CMake generator, same as CMake format|

---
+ ### `!MakeNew <项目名称> [-install: mod名称] [-message: 项目说明] [-vcpkg: 额外依赖项]`  
用于快速新建符合此工作项目规格的插件项目.  
参数 | 说明
--- | ---
`项目名称` | 项目名称, 同步于`CMakeLists.txt`, `vcpkg.json`和解决方案内.
`mod名称` | 可选, `-install`或`-i`, 指定MO2安装时的mod名称(默认为项目名称). 用单引号`'`包含.
`项目说明` | 可选, `-message`或`-m`, 简易的项目说明, 生成于附属的`vcpkg.json`文件内. 用单引号`'`包含.
`额外依赖项` | 可选, `-vcpkg`或`-v`, 额外的`vcpkg`依赖项. 默认为`spdlog`. 支持`feature`格式. 多个依赖项用逗号分隔. 

---
+ ### `!Update <运行模式>`  
用于编译后复制文件, 为`CMakeLists.txt`生成源文件表, 以及自动更新. 在正常使用此工作项目时, 此脚本会被自动更新到每一个下属项目中并嵌入至生成的解决方案中, 不需要单独手动运行.  
参数 | 说明
--- | ---
`COPY` | 由VS编译后事件调用, 需求多个参数. 
`DISTRIBUTE` | 由`!Rebuild`脚本调用, 发布`!Update`脚本的最新版至每一个下属的插件项目文件夹中.
`SOURCEGEN` | 由CMake生成器调用, 生成当前项目的源文件表.  

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
