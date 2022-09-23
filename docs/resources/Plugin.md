<h1 align="center">插件基础</h1>  
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

本节教程演示如何生成一个解决方案, 编译插件并加载到游戏里获得日志反馈.  

## 解决方案

按照[工具配置](/docs/setup/Setup.md)搭建好`SKSEPlugins`开发环境并完成必需的`-BOOTSTRAP`步骤后, 使用`!MakeNew MyNewPlugin`脚本生成一个名为`MyNewPlugin`的插件项目, 随后通过`!Rebuild`脚本生成解决方案并预编译`CLib`静态库, 以节省后期编译的时间.  
```powershell
cd .\SKSEPlugins
.\!makenew MyNewPlugin
.\!rebuild flatrim
```
`flatrim`指代除VR以外的所有版本.  
![rebuild_pt1](/images/resources/rebuild_pt1.png)  
![rebuild_pt2](/images/resources/rebuild_pt2.png)  

## 项目构成

生成结束后, 打开`SKSE64_FLATRIM.sln`并定位到`Plugins\MyNewPlugin`项目，这便是我们插件项目.  
![mynewplugin_project](/images/resources/mynewplugin_project.png)  
+ `\include`: 包含插件信息和插件加载方法的头文件, 均为自动生成
+ `\Precompile Header File`: CMake项目自动生成, 包含预编译头(`.hxx`)
+ `\Source Files`: CMake项目自动生成, 包含预编译头的编译单元(`.cxx`)
+ `\src`: 插件项目实际源码位置, 对于插件的开发都会在此操作
+ `.clang-format`: 代码风格格式文件
+ `CMakeLists.txt`: CMake项目文件
+ `vcpkg.json`: `vcpkg`依赖库清单, 以及插件项目的mod安装信息  

打开`main.cpp`可以看见如下结构: 
![plugin_main](/images/resources/plugin_main.png)  
如果使用的是默认的`CommonLibSSE-NG`, 则移除掉行9处的`REL::Module::reset();`. 这是一个CLib-NG库的bug的暂时修复, 但默认CLib-NG库将其放在了单元测试代码块里, 此处暂时带过. 若使用MaxSu的CLib库NG分支, 可以保持原样, 因为MaxSu的CLib库NG分支移除了单元测试限定.  
```cpp
DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
```
这是SKSE插件项目的加载入口, 类似于普通dll的`DLLMAIN`, 当SKSE插件被skse_loader加载时, 会据插件名字顺序依次执行各个插件的`SKSEPlugin_Load`函数.  
```cpp
#ifndef NDEBUG
	while (!IsDebuggerPresent()) { Sleep(100); }
#endif
```
这是用于调试(debug)插件的语句, 会在插件被加载时进入等待循环, 以保证插件作者有足够的时间附加调试器到游戏进程上并加载插件项目的调试符号. 此处我们暂时将`while`语句注释掉, 具体的调试步骤后面再展开.  
```cpp
DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());
SKSE::Init(a_skse);
INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);
```
这部分代码用于加载logger并初始化插件内部的SKSE接口, 以确保插件可以正确的与SKSE交互. 随后打印一句标准log表示logger加载完毕.  
```cpp
// do stuff

return true;
```
最后这部分代码, `// do stuff`注释后则是我们实际进行插件操作的地方, 譬如注册SKSE消息回调函数, 加载配置文件, 启用内存补丁等. 当一切操作成功后, 则为SKSE返回`true`, 反之则返回`false`向SKSE汇报插件加载失败.  

## 日志宏

使用`SKSEPlugins`脚本部署的插件开发环境可以使用`INFO()`, `DEBUG()`, 和`ERROR()`宏来输出log语句, 依照`std::fmt`的格式.  
```cpp
INFO("INFO语句, 插件名 {}, 加载成功: {}", Plugin::NAME, true);
DEBUG("DEBUG语句, Release模式下未启用`DEBUG LOG`则不会输出DEBUG语句");
ERROR("致命错误语句, 会弹出当前代码部分的详细信息并中止游戏进程");
```
善用日志宏, 对于插件开发和纠错排bug有很大的帮助.  

## 编译与部署

按下`Ctrl+B`编译插件, 在生成事件中选择复制到游戏Data(`Copy to Data`)或安装至MO2(`Copy to MO2`).  
![plugin_postbuild](/images/resources/plugin_postbuild.png)  
启动游戏, 加载完毕后打开SKSE log目录下的`MyNewPlugin.log`  
![plugin_log](/images/resources/plugin_log.png)

自此一个非常基础的SKSE插件项目就完成了从生成到编译到加载的全部步骤.  

## 消息回调

开发插件的过程中, 必然会遇到插件的功能不能在`SKSEPlugin_Load`处执行, 即不能在SKSE加载插件时就立刻执行, 此时很多游戏内数据并未初始化, 很多函数也并未加载, 因此需要注册一个SKSE消息回调函数, 在SKSE加载游戏的各个阶段分批次执行我们的回调函数.  

### 消息处理

首先我们准备一个符合SKSE标准的消息回调函数:  
```cpp
// @ main.cpp
void MessageHandler(SKSE::MessagingInterface::Message* a_msg) noexcept
{
    if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
        // do callback stuff
        INFO("This is a callback after data loaded!");
    }
}
```
这是最常见的一种回调, 它的触发条件为当`SKSE`加载完所有游戏资源后(`kDataLoaded`), 对于游戏各种类和数据的调用/修改都应当于此处或之后执行.  
当需要注册多种条件的回调时, 则可以将`if`语句转换为`switch (a_msg->type)`语句, 并使用SKSE提供的以下条件:  
```cpp
kPostLoad
kPostPostLoad
kPreLoadGame
kPostLoadGame
kSaveGame
kDeleteGame
kInputLoaded
kNewGame
kDataLoaded
```

### 注册回调

在`SKSEPlugin_Load`函数内使用SKSE提供的消息接口来注册我们的消息回调:  
```cpp
// @ main.cpp @@ SKSEPlugin_Load
if (!SKSE::GetMessagingInterface()->RegisterListener(MessageHandler)) {
    return false;
}
```
若注册失败, 则依据SKSE加载规则返回`false`跳过加载我们的插件.  

### 示例
```cpp
// @ main.cpp
namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
        // 数据加载完毕后, 执行Form修改操作
		if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
			Forms::PatchAll();
		}
	}
}


DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
#ifndef NDEBUG
	while (!IsDebuggerPresent()) { Sleep(100); }
#endif

	DKUtil::Logger::Init(Plugin::NAME, REL::Module::get().version().string());

	SKSE::Init(a_skse);
	
	INFO("{} v{} loaded", Plugin::NAME, Plugin::Version);

	// 加载配置文件
	Config::Load();

    // 启用内存补丁
	if (*Config::EnableUE) {
		Hooks::Install();
	}

    // 注册回调
	const auto* message = SKSE::GetMessagingInterface();
	if (!message->RegisterListener(MessageHandler)) {
		return false;
	}

	return true;
}
```

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
