# 示例插件
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [示例插件](/docs/PluginTemplate.md) | [常用方法](/docs/CommonMethods.md) | [版本集成](/docs/AddressLibrary.md) | [探索未知](/docs/ToUnknown.md) | [CommonLibSSE](/docs/CommonLibSSE.md)

## 基础

制作插件的第一步, 是先新建一个项目. 右击解决方案, 选择`添加->新建项目`, 在窗口中选中`动态链接库(DLL) C++`并继续下一步. 命名并确定创建路径在`src\skse64`后, 创建即可. (教程将会命名插件项目为`PluginTemplate`, 下文简称`"PluginTemplate"`)  
![AddProject](/images/sln_add.png)

接下来, 右击解决方案, 选择`添加->新建解决方案文件夹`, 命名为`SKSE`, 然后将所有的来自SKSE组的项目拖拽进去---这一步并不是必需的, 然而良好的文件管理可以在插件项目繁杂时给予非常正向的帮助. 图例:  
![SolutionFolder](/images/sln_folder.png)

需要对插件项目的属性进行一些修改, 如下: (**确保`配置`为`所有配置`**)
```
常规->常规属性->C++语言标准  *下拉  |  ISO C++17标准(std:c++17)
高级->高级属性->字符集  *下拉  |  使用多字节字符集
C/C++->常规->附加包含目录  *复制  |  $(SolutionDir);$(SolutionDir)..;$(ProjectDir)include;%(AdditionalIncludeDirectories)
     ->常规->SDL检查  |  *清空此选项
     ->代码生成->运行库  *下拉  |  多线程(/MT)
     ->代码生成->启用函数级链接  *下拉  |  是(/Gy)
     ->语言->符合模式  *下拉  |  否
     ->语言->强制类型转换规则  *下拉  |  是(/Zc:rvalueCast)
     ->预编译头->使用预编译头  |  *清空此选项
     ->预编译头->预编译头文件  *复制  |  stdafx.h
     ->高级->编译为  *下拉  |  编译为C++代码(/TP)
     ->高级->强制包含文件  *复制  |  common/IPrefix.h;%(ForcedIncludeFiles)
     ->高级->使用完全路径  *下拉  |  否
链接器->输入->模块定义文件  *复制  |  exports.def
```
修改完成后选择应用并确定. 这是基础的项目模板, 可酌情修改.

## 依赖

SKSE64插件有两种开发方式, 一种是使用传统的SKSE64库 + common_vc14进行开发, 另一种则是使用Ryan的CommonLibSSE库 + 部分SKSE64进行开发. 这篇教程会先以前者为主讲解插件开发的基础知识, 之后则会比较CommonLibSSE与SKSE64, 并以后者做出示例. 当然也可以两个库结合，但结果通常会是插件体积膨胀.

选择一种开发方式很简单, 只需将所对应的库添加进项目引用即可.  
![AddReference](/images/proj_add_ref.png)

这一部分教程会以SKSE64做讲解, 因此只需要在项目引用窗口中勾选`skse64`并应用即可.

## 结构

一个SKSE64插件的结构分为两部分:

第一部分由插件作者发挥创造力, 根据SKSE64库提供的内容对游戏内容进行自由修改, 通常包含插件作者定义的类/结构, 方法, 成员, Hook等内容.

第二部分则由SKSE64统一定义, 由插件作者遵守约定的SKSE方法完成插件的信息校验和加载功能`Query & Load`, 这样SKSE64启动游戏时才能正确的加载插件, 也为之后顺利运行第一部分的代码提供基础.

## 开发

***
### 添加文件

首先确保由Visual Studio创建的默认项目文件已被删除---如`pch.h`, `pch.cpp`等默认情况下Visual Studio为DLL项目新建的一些文件. 这些文件于SKSE64插件基本无用, 切换为全文件视图后删除即可. (直接删除只会删除引用)  
![ShowAllFiles](/images/sln_showall.png)

> 全文件视图: 默认情况下Visual Studio会显示过滤器视图, 这样操作的文件都是引用地址, 添加文件无法添加到目标物理文件夹中.

在全文件视图下右击插件项目, 选择`添加->新建文件夹`并创建两个文件夹, 分别命名为`src`和`include`. 这是最传统的C++项目命名方式. 在教程中所有的`.cpp`源文件将会被放置在`src`中, 而所有的`.h`头文件则会被放置在`include`中.

再次右击插件项目, 选择`添加->新建项`(下文简称新建), 在左侧`Visual C++/代码`选择`模块定义文件(.def)`并命名为`exports.def`. (稍后会解释作用)

在`src`文件夹为其新建`C++文件(.cpp)`并命名为`main.cpp`. 这将是插件的入口.

在`include`文件夹为其新建`头文件(.h)`并命名为`version.h`. 这将是插件的版本信息.

完成后, 项目应当如下图所示:  
![ProjectFirstView](/images/proj_first.png)

***
### 基础代码

#### exports.def

模块定义文件(.def)负责定义一个DLL的**导出**信息. 可以理解为只有这个文件里面导出的函数成员才能在动态链接时调用---也就是当SKSE64启动游戏, 加载插件时, 只有被导出的函数才能被外部调用.

正如教程之前所提到的, 插件作者遵守约定的SKSE方法来进行插件的信息查询和加载. 因此需要在`exports.def`里导出约定好的两个函数.

打开`exports.def`文件, 添加如下定义:
```C++
LIBRARY "PluginTemplate"
EXPORTS
SKSEPlugin_Query
SKSEPlugin_Load
```
`LIBRARY`关键字定义此动态链接库(DLL)的名称. (`PluginTemplate`是教程所用的示例插件项目名; 用双引号包围)  
`EXPORTS`关键字定义导出的函数名称. SKSE64插件需要使用SKSE64约定的导出函数名称, `SKSEPlugin_Query`和`SKSEPlugin_Load`. 这两个函数分别负责信息校验和插件加载.

> 其它函数不应导出.

#### version.h

打开`version.h`文件, 添加如下代码:
```C++
#pragma once

constexpr auto PLTP_VERSION = "1.0.0.0";
```
这里用了**预处理指令**`#pragma once`(通常情况下, Visual Studio会自动为头文件添加此预处理器). 预处理指令和C++语言无关, 因此无需放置分号在句尾. 顾名思义, 它们会在编译前被转化成相应的操作. 预处理指令都以井号`#`开头.

`#pragma once`是告诉编译器这个文件不管被引用多少次, 只会被编译一次. (在头文件被引用很多次的时候, 重复编译会引发错误)

`constexpr auto PLTP_VERSION = "1.0.0.0";`表示这将会是一个已知且固定的常量表达式(`constexpr`), 类型则由编译器自动推导(`auto`), 名称为`PLTP_VERSION`, 赋值为`"1.0.0.0"`

#### main.cpp

打开`main.cpp`, 添加如下代码:
```C++
#include "version.h"

#include <shlobj.h>	

#include "skse64/PluginAPI.h"
#include "skse64_common/skse_version.h"
```
这里**引用**了四个头文件, 分别为`version.h`(由上一步创建), `shlobj.h`(这是一个标准头文件), `PluginAPI.h`(位于`skse64`文件夹中)和`skse_version.h`(位于`skse64_common`文件夹中).
+ `"version.h"`里面包含着插件本身的版本信息(`PLTP_VERSION`).
+ `<shlobj.h>`由尖括号包围而不是双引号, 代表这个头文件由安装的C++工具集提供. 可以从这里获取用户文件夹的相对路径.
+ `"PluginAPI.h"`这是SKSE64提供的插件接口, 可以从这里获取各种SKSE64接口.
+ `"skse_version.h"`这是由SKSE64提供的版本信息, 可以从这里获取当前游戏版本以校验自身插件是否兼容.

> 引用头文件: 引用一个头文件其实就是把这个头文件的内容隐式复制到当前位置, 使得当前文件拥有所引用头文件的内容. 复制步骤是编译时才会执行且不可见.

> 引用方法: 使用`#include`预处理指令即可, 通常写于文件起始(顶部). 不止源文件可以引用, 头文件也可以引用其它的头文件.

在引用之后添加如下代码:
```C++
extern "C"
{
}
```
由于不同IDE的编译器或不同版本的编译器(教程所用为Visual Studio, MSVC编译器)所生成的代码不同, 在运行时无法确保链接库的统一性, 因此用`extern "C"`使编译器将此作用域内的内容编译为符合`"C"`标准约定的代码. `extern "C"`作用域的范围由一对花括号限定. (下文简称"导出域")

完成两个约定函数的补全(`SKSEPlugin_Query`和`SKSEPlugin_Load`), 在导出域内添加如下代码:
```C++
bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
{
    IDebugLog::OpenRelative(CSIDL_MYDOCUMENTS, R"(\My Games\Skyrim Special Edition\SKSE\PluginTemplate.log)");
    IDebugLog::SetPrintLevel(IDebugLog::kLevel_Error);
    IDebugLog::SetLogLevel(IDebugLog::kLevel_DebugMessage);

    _MESSAGE("PluginTemplate %s", PLTP_VERSION);

    a_info->infoVersion = PluginInfo::kInfoVersion;
    a_info->name = "PluginTemplate";
    a_info->version = 1;

    if (a_skse->isEditor) {
        _FATALERROR("loaded in editor, marking as incompatible");

        return false;
    }

    if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_97) {
        _FATALERROR("unsupported runtime version %08x", a_skse->runtimeVersion);

        return false;
    }

        return true;
}
```
`SKSEPlugin_Query`是第一个SKSE64所需的函数, 返回类型为`bool`(是/否), 参数类型为`const SKSEInterface*`和`PluginInfo*`, 对应的参数名称为`a_skse`和`a_info`(教程中所有参数都会以`a_`前缀表明这是一个参数). 当SKSE64启动时, 会首先执行每一个插件的此函数以校验插件自身是否兼容.

`IDebugLog`三排代码首先确定日志文件的路径`\My Games\Skyrim Special Edition\SKSE\PluginTemplate.log`, 日志输出的等级`IDebugLog::kLevel_Error`和日志记录的等级`IDebugLog::kLevel_DebugMessage`.

`_MESSAGE`日志函数打印(下文简称"打印")此插件的名称`"PluginTemplate"`, 后接`PLTP_VERSION`插件版本信息(会替换`%s`输出控制符).

`a_info`三排代码为其成员`infoVersion`, `name`和`version`赋值. 应总是为`infoVersion`赋值`PluginInfo::KInfoVersion`, 为`name`赋值此插件的名称, 为`version`赋值此插件的版本信息(数字).

第一个`if`语句检查SKSE64是否处于编辑器模式, 即检查`a_skse`其成员`isEditor`值是否为`true`. 若值为`true`, 会以`_FATALERROR`打印致命错误`"loaded in editor, marking as incompatible"`, 表明插件于编辑器模式中被加载, 此次加载不兼容. 最后返回`false`表明插件校验失败. 若值为`false`, 此`if`语句将跳过.

第二个`if`语句检查SKSE64的版本, 即检查`a_skse`其成员`runtimeVersion`是否不等于**`RUNTIME_VERSION_1_5_97`**. 若不等于, 会以`_FATALERROR`打印致命错误`"unsupported runtime version"`, 后接`runtimeVersion`当前游戏版本信息(会替换`%08x`输出控制符), 表明插件所需版本信息和当前游戏版本不兼容. 最后返回`false`表明插件校验失败. 若版本信息相等, 此`if`语句将跳过.

当上述步骤皆校验成功后, 应当返回`true`表明插件校验成功. 若返回`false`, SKSE64会停止校验并中断游戏.

> 此函数中不应执行除校验和获取接口实例之外的操作.

> 教程使用游戏版本`1.5.97`, 插件作者应适当修改为符合的版本.

在函数结束后新增两次换行, 添加如下代码: 
```C++
bool SKSEPlugin_Load(const SKSEInterface* a_skse)
{
    _MESSAGE("PluginTemplate loaded");

    _MESSAGE("Hello SKSE64 !");

    return true;
}
```
`SKSEPlugin_Load`是第二个SKSE64所需的函数, 返回类型为`bool`(是/否), 参数类型为`const SKSEInterface*`, 对应的参数名称为`a_skse`. 当SKSE64校验完所有的插件后, 便会执行每一个插件的此函数以完成插件的加载.

第一句打印`"PluginTemplate loaded"`, 表明此插件已开始加载.

第二句打印`"Hello SKSE64 !"`, 表明此插件正常工作.

最后返回`true`表明插件加载成功. 若返回`false`, SKSE64会跳过加载此插件, 但不会中断游戏.

> 在此函数中应执行如加载配置文件, 修改游戏数据, 导出Papyrus函数, 注册消息接口等操作.

> 确保两个函数都写于导出域内(花括号内).

***
### 作者注:

> `main.cpp`示例可以[在这里](/examples/PluginTemplate/src/main.cpp)查看.

> [预处理指令及分类](http://c.biancheng.net/view/286.html)

> [C++声明和定义的区别](https://blog.csdn.net/a8039974/article/details/90697461)

> [C++中的箭头运算符的含义](https://blog.csdn.net/yangyong0717/article/details/73693496)

> C++并不是一门简单的语言, 相反作为强类型语言的典范, C++更加需要正确的符号, 语法和代码规范. 此教程会在涉及到的地方尽量普及一些C++知识, 更详细的C++语法知识则需要自行学习. 请善用搜索引擎!

## 编译
+ 配置: `Debug`配置下会生成调试符号库(`.pdb`), `Release`配置下享有最大化优化(小体积). 教程会使用`Debug`配置.
+ 确保目标平台为`x64`.

插件项目的编译依赖于SKSE64的静态链接库, 因此需要先编译SKSE64项目, 分别为`common_vc14`, `skse64`, `skse64_common`. 若已为SKSE64项目分好文件夹, 此时只需要在解决方案界面右击该文件夹, 选择`生成`即可. 或者选中这三个项目右击并选择`生成`.  
![BuildProjects](/images/proj_build.png)

SKSE64编译完成后, 右击插件项目并选择`生成`.

> 编译SKSE64项目时, 可能会遇到`静态断言失败`错误. 此错误不会中断编译过程.  
> *这是因为静态断言在编译前执行, 此时Visual Studio还不能正确的判断断言的对象大小.*

> 可以在`Debug`和`Release`两个配置下分别编译一次SKSE64项目.
> 此步骤是一次性的, 编译好的SKSE64项目可以为插件编译节约大量的时间.

## 调试

***
### 路径设置

调试SKSE64插件首先需要将其挂载到SKSE64的插件文件夹内. 教程会示例如何使用`生成后事件`属性自动拷贝编译后插件至游戏文件夹.

右击插件项目并选择`属性`, 点击`生成事件->生成后事件->命令行`属性的下拉框并选择`<编辑>`, 填写如下:
```Batch
copy /y "$(TargetPath)" "游戏本体路径\Data\SKSE\Plugins\$(TargetFileName)"
copy /y "$(TargetDir)$(TargetName).pdb" "游戏本体路径\Data\SKSE\Plugins\$(TargetDir)$(TargetName).pdb"
```
将`游戏本体路径`替换为实际路径并确保`生成事件->生成后事件->在生成中使用`已设为`是`.

> 示例命令: `copy /y "$(TargetPath)" "D:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition\Data\SKSE\Plugins\$(TargetFileName)"`

> 若使用MO2:  
> ```Batch
> if not exist "MO2基准目录\mods\$(TargetName)\SKSE\Plugins\" mkdir "MO2基准目录\mods\$(TargetName)\SKSE\Plugins\"
> copy /y "$(TargetPath)" "MO2基准目录\mods\$(TargetName)\SKSE\Plugins\$(TargetFileName)"
> copy /y "$(TargetDir)$(TargetName).pdb" "MO2基准目录\mods\$(TargetName)\SKSE\Plugins\$(TargetName).pdb"
> ```
> 将`MO2基准目录`替换为实际路径并确保`生成事件->生成后事件->在生成中使用`已设为`是`.

> 如何寻找MO2基准目录: (`/`与`\`皆可作为地址分隔符)  
> ![MO2Path](/images/mo2_path.png)

> 当复制到MO2目录后, 需要在MO2界面按下`F5`进行刷新.

***
### 调试方法

<dl>
    <dt>日志</dt>
    <dd>在游戏内观察插件的效果或打印日志.</dd>
    <dt>断点</dt>
    <dd>使用Visual Studio进行单步调试.</dd>
</dl>

日志调试法是最简单, 最直接的调试方法. 通过打印特定数据和条件语句, 日志可以给予非常直接的反馈. 例如: 需要获取函数`MyFunc`的内存地址, 可直接在函数`SKSEPlugin_Load`内打印:
```C++
_DMESSAGE("%p", &MyFunc);
```
当插件加载时, 便会打印函数`MyFunc`的地址.

> 断点调试法需要对Visual Studio有一定程度的了解. 通过使用`Debug`配置下编译的`skse64_1_5_97.dll`并附加Visual Studio调试器至游戏进程, 可以击中提前设定好的代码断点并观察各项值的变化.  
![StepDebugger](/images/step_debug.png)

> 为什么不打印中文日志? 因为中文日志有可能遇到编码错误.

> 使用单步调试时, 应当将插件挂载到游戏本体而非MO2路径, 并通过`skse64_loader.exe`启动.

> 当涉及到单步调试时, 教程会使用X64DBG进行断点, 而不是Visual Studio.

***
### [Hello SKSE64 !]

通过`skse64_loader.exe`(或MO2的`SKSE`快捷方式)启动游戏. 当游戏主菜单正常显示后, 代表没有致命错误(`_FATALERROR`)发生, 此时可以切出游戏检查日志目录: `~我的文档\My Games\Skyrim Special Edition\SKSE`. 打开`PluginTemplate.log`, 可以看到如下内容:  
![PluginSuccess](/images/plugin_success.png)

教程至此, 插件作者应掌握了基本的SKSE64插件项目.

***
##### [回到目录](../README.md) | [开发环境](/docs/Setup.md) | [示例插件](/docs/PluginTemplate.md) | [常用方法](/docs/CommonMethods.md) | [版本集成](/docs/AddressLibrary.md) | [探索未知](/docs/ToUnknown.md) | [CommonLibSSE](/docs/CommonLibSSE.md)