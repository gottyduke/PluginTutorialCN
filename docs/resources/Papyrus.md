<h1 align="center">Papyrus调用</h1>  
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

本节教程演示如何在插件注册一个原生函数(`native`)到游戏中供Papyrus脚本调用.  

## SKSE

### 函数原型

首先我们准备符合SKSE标准的Papyrus原生函数:  
```cpp
std::string GetText(RE::StaticFunctionTag*)
{
    return "Hello Papyrus"s;
}

std::string GetIntText(RE::StaticFunctionTag*, std::int32_t a_int)
{
    return fmt::format("Hello Papyrus, this is {}", a_int);
}
```
函数很简单, `GetText()`被Papyrus调用时, 返回一个内容为`Hello Papyrus`的字符串. 而`GetIntText(Int)`被调用时, 返回一个内容为`Hello Papyrus`及`Int`参数转化的字符串. Papyrus原生函数在CLib中以`RE::StaticFunctionTag*`作为第一个参数, 即使这个函数本身并不接受参数. 类似于成员函数都以`this`作为第一个参数.  

### 注册Papyrus虚拟机

有了Papyrus原生函数后, 便需要把这个函数注册到游戏内部的Papyrus虚拟机中, 这样我们的Papyrus脚本就能以此调用我们SKSE插件为其注册的函数. SKSE注册Papyrus函数分为三部分, 1) 获取SKSE提供的Papyrus接口; 2) 通过Papyrus接口注册Papyrus虚拟机处理函数; 3) 通过Papyrus虚拟机处理函数注册我们的Papyrus原生函数至Papyrus虚拟机内.  
以下为示例代码:  
```cpp
namespace
{
	std::string GetText(RE::StaticFunctionTag*)
	{
		return "Hello Papyrus"s;
	}

	std::string GetIntText(RE::StaticFunctionTag*, std::uint32_t a_int)
	{
		return fmt::format("Hello Papyrus, this is {}", a_int);
	}

	// 3) 注册我们的Papyrus原生函数
	bool PapyrusVMHandler(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("SKSE_GetText", "MyNewPlugin_Native", GetText);
		a_vm->RegisterFunction("SKSE_GetIntText", "MyNewPlugin_Native", GetIntText);

		return true;
	}

	void MessageHandler(SKSE::MessagingInterface::Message* a_msg) noexcept
	{
		if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
			// 1) 获取SKSE提供的Papyrus接口
			auto* papyrus = SKSE::GetPapyrusInterface();
			// 2) 注册Papyrus虚拟机处理函数
			papyrus->Register(PapyrusVMHandler);
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

	// do stuff
	if (!SKSE::GetMessagingInterface()->RegisterListener(MessageHandler)) {
		return false;
	}

	return true;
}
```
Papyrus虚拟机提供的`RegisterFunction`用于将一个C++原生函数注册到Papyrus虚拟机中, 并将其与Papyrus内对应的函数名字及类名字绑定.  
```cpp
// 将GetText注册为SKSE_GetText, 类名为MyNewPlugin_Native
a_vm->RegisterFunction("SKSE_GetText", "MyNewPlugin_Native", GetText);
// 将GetIntText注册为SKSE_GetIntText, 类名为MyNewPlugin_Native
a_vm->RegisterFunction("SKSE_GetIntText", "MyNewPlugin_Native", GetIntText);
```

### 调用Papyrus原生函数

在我们的`MyNewPlugin_Native.psc`中加入:  
```papyrus
Scriptname MyNewPlugin_Native

String Function GetText() native
String Function GetIntText(Int aiNum) native
```
按照Papyrus脚本格式调用即可(`Scriptname XXX extends MyNewPlugin_Native`, `native global`等).

### Papyrus类型与C++类型对照  

一些**常用**的类型: 
|Papyrus|C++|
|-|-|
|`Int`|`std::int32_t`|
|`Float`|`float`|
|`String`|`std::string`或`RE::BSFixedString`|
|`Bool`|`bool`|
|`Actor`|`RE::Actor*`|
|`Faction`|`RE::TESFaction*`|
|`Form`|`RE::TESForm*`|
|`ObjectReference`|`RE::TESObjectREFR*`|
|`Quest`|`RE::TESQuest*`|
|`ReferenceAlias`|`RE::BGSRefAlias*`|
|`Shout`|`RE::TESShout*`|
|`Spell`|`RE::SpellItem*`|  

当Papyrus类型为数组时, C++类型为编译期已知`std::array`或动态`std::vector`.  
当Papyrus脚本传入数组参数时, C++类型为`const RE::reference_array`.

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
