# 常用方法->插件接口->消息队列
#####  [常用方法](/docs/CommonMethods.md) | [消息队列](/docs/CM/Interface/Messaging.md) | [Papyrus对象](/docs/CM/Interface/Object.md) | [Papyrus脚本](/docs/CM/Interface/Papyrus.md) | [Scaleform界面](/docs/CM/Interface/Scaleform.md) | [序列化](/docs/CM/Interface/Serialization.md) | [代理委托](/docs/CM/Interface/Task.md)

SKSE64在加载过程中会根据当前状态广播对应消息, 这个过程通过消息队列接口`SKSEMessagingInterface`实现. 如果希望收到特定消息以执行特定的操作, 应当以自身句柄注册消息队列.  
![InterfaceMessaging](..images/intrfc_messaging.png)

以下示例如何为示例插件项目`PluginTemplate`注册消息队列, 并在收到特定消息后打印日志.

## 处理消息

在匿名空间内新增如下代码:
```C++
void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{	
    if (a_msg->type == SKSEMessagingInterface::kMessage_DataLoaded) {
        _DMESSAGE("Data Loaded");
    }
}
```
函数`MessageHandler`负责处理收到的消息, 参数类型为`Message*`. 函数内使用了`if`语句判断消息类型`a_msg->type`是否为`kMessage_DataLoaded`(游戏数据加载完毕). 收到此消息后会打印日志`"Data Loaded"`表明数据已加载完毕. 若收到其它类型的消息, 此`if`语句跳过.

## 注册聆听

在函数`SKSEPlugin_Load`末尾语句前添加如下代码:
```C++
if (g_messaging->RegisterListener(g_thisPlugin, "SKSE", MessageHandler)) {
    _MESSAGE("Registered messaging interface");
} else {
    _MESSAGE("Failed to register messaging interface");

    return false;
}
```
其中`g_messaging`为此插件预先声明的消息接口实例.

函数`RegisterListener`返回值为`bool`, 参数类型为`PluginHandle`, `const char*`和`EventCallback`. 

应为`PluginHandle`传入此插件句柄`g_thisPlugin`, 为`const char*`(字符串)传入消息发送者名称的字面值(此处为`"SKSE"`代表SKSE64), 为`EventCallback`传入一个符合消息处理规范的函数(此处为`MessageHandler`代表期望执行的消息处理函数).

函数`RegisterListener`会根据是否成功注册接收而返回`true`/`false`. 应以此返回值决定是否继续加载此插件.

当注册的消息发送者(`"SKSE"`)广播消息, 申请聆听的插件(`g_thisPlugin`)便会执行其注册的消息处理函数(`MessageHandler`). 若函数返回值为`true`, 会打印日志`"Registered messaging interface`表明注册成功. 若函数返回值为`false`, 会打印日志`Failed to register messaging interface`并中断此次加载.

> 完整示例可以[在这里](/examples/Interface/Messaging.cpp)查看.

## 插件通信

若期望聆听来自其它插件的消息, 或广播消息至注册的聆听者

***
#####  [常用方法](/docs/CommonMethods.md) | [消息队列](/docs/CM/Interface/Messaging.md) | [Papyrus对象](/docs/CM/Interface/Object.md) | [Papyrus脚本](/docs/CM/Interface/Papyrus.md) | [Scaleform界面](/docs/CM/Interface/Scaleform.md) | [序列化](/docs/CM/Interface/Serialization.md) | [代理委托](/docs/CM/Interface/Task.md)