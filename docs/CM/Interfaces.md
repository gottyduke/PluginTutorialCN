# 常用方法->插件接口
##### [回到上层](/docs/CommonMethods.md) | [插件接口](/docs/CM/Interfaces.md) | [响应事件](/docs/CM/Events.md) | [游戏数据](/docs/CM/Data.md) | [内存挂钩](/docs/CM/Hooks.md)

## 接口目录

SKSE64提供了接口基类(`SKSEInterface`), 其派生类如下:
接口 | 类型 | 示例
--- | --- | ---
`SKSEMessagingInterface` | 消息队列 | [施工中](/docs/CM/Interfaces/Messaging.md)
`SKSEObjectInterface` | Papyrus对象 | [施工中](/docs/CM/Interfaces/Object.md)
`SKSEPapyrusInterface` | Papyrus脚本 | [施工中](/docs/CM/Interfaces/Papyrus.md)
`SKSEScaleformInterface` | Scaleform界面 | [施工中](/docs/CM/Interfaces/Scaleform.md)
`SKSESerializationInterface` | 序列化 | [施工中](/docs/CM/Interfaces/Serialization.md)
`SKSETaskInterface` | 代理委托 | [施工中](/docs/CM/Interfaces/Task.md)

> 请先掌握如何获取接口实例(下文), 再阅读相关示例.

## 匿名空间

使用接口需要提前声明其插件句柄和期望使用的接口为全局变量, 并初始化为空值.

打开`main.cpp`文件, 在`extern`关键字前添加如下代码:
```C++
namespace 
{
}
```
`namespace`关键字声明一个命名空间, 此处使用不加名称的命名空间(下文简称"匿名空间")是为了更好的管理全局变量和函数. 命名空间的范围由一对花括号限定.

> [C++匿名命名空间](https://www.cnblogs.com/youxin/p/4308364.html)

> 此步骤并非必需. 可按个人风格声明全局变量.

## 插件句柄

在匿名空间内声明接口:
```C++
PluginHandle g_thisPlugin = kPluginHandle_Invalid;
```
`g_thisPlugin`为此插件的句柄(`PluginHandle`), 应初始化值为`kPluginHandle_Invalid`.

在函数`SKSEPlugin_Query`内获取插件句柄: 
```C++
g_thisPlugin = a_skse->GetPluginHandle();
```
函数`GetPluginHandle`的返回值便是当前插件的句柄, 应将其储存于预先声明的插件句柄`g_thisPlugin`中.

不应在函数`SKSEPlugin_Query`或函数`SKSEPlugin_Load`以外使用`GetPluginHandle`; 否则返回值无效.

## 获取接口

以消息队列接口`SKSEMessagingInterface`为例, 在匿名空间内声明接口并初始化为空指针: 
```C++
SKSEMessagingInterface* g_messaging = nullptr;
```
`g_messaging`为此插件的消息队列接口(`SKSEMessagingInterface`), 应初始化值为`nullptr`(空指针).

在函数`SKSEPlugin_Query`内获取接口实例:
```C++
g_messaging = static_cast<SKSEMessagingInterface*>(a_skse->QueryInterface(kInterface_Messaging));
```
通过`a_skse`成员函数`QueryInterface`获取`kInterface_Messaging`接口(消息队列), 并将其返回值**静态类型转换**为`SKSEMessaginInterface*`. 将结果储存于`g_messaging`后便可使用该对象.

> 期望使用其它接口时, 依此示例通过类型转换获取相应对象.
在匿名空间内声明接口并初始化为空指针,

函数`QueryInterface`参数:
接口实例 | 类型 | 参数
--- | --- | ---
`SKSEMessagingInterface*` | 消息队列 | `kInterface_Messaging`
`SKSEObjectInterface*` | Papyrus对象 | `kInterface_Object`
`SKSEPapyrusInterface*` | Papyrus脚本 | `kInterface_Papyrus`
`SKSEScaleformInterface*` | Scaleform界面 | `kInterface_Scaleform`
`SKSESerializationInterface*` | 序列化 | `kInterface_Serialization`
`SKSETaskInterface*` | 代理委托 | `kInterface_Task`

> [C++类型转换](https://blog.csdn.net/qq_40421919/article/details/90677220)

***
##### [回到上层](/docs/CommonMethods.md) | [插件接口](/docs/CM/Interfaces.md) | [响应事件](/docs/CM/Events.md) | [游戏数据](/docs/CM/Data.md) | [内存挂钩](/docs/CM/Hooks.md)