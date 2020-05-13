# 常用方法->插件接口->消息队列

##### [回到上层](/docs/CM/Interfaces.md) | [消息队列](/docs/CM/Interfaces/Messaging.md) | [Papyrus 对象](/docs/CM/Interfaces/Object.md) | [Papyrus 脚本](/docs/CM/Interfaces/Papyrus.md) | [Scaleform 界面](/docs/CM/Interfaces/Scaleform.md) | [序列化](/docs/CM/Interfaces/Serialization.md) | [代理委托](/docs/CM/Interfaces/Task.md)

SKSE64 在加载过程中会根据当前状态广播对应消息, 这个过程通过消息队列接口`SKSEMessagingInterface`实现. 如果希望收到特定消息以执行特定的操作, 应当以自身句柄注册消息队列.  
![InterfaceMessaging](/images/intrfc_messaging.png)

以下示例如何为教程插件项目`PluginTemplate`注册消息队列, 并在收到特定消息后打印日志.

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
函数`RegisterListener`: 此函数负责注册插件聆听至消息队列中.
参数 | 类型
--- | ---
`PluginHandle` | 聆听者插件句柄
`const char*` | 广播者插件名称
`EventCallback` | 消息处理回调函数
返回值 | `bool`

应总是为`PluginHandle`传入此插件句柄`g_thisPlugin`, 为`const char*`(字符串)传入消息发送者名称的字面值(此处为`"SKSE"`代表 SKSE64), 为`EventCallback`传入一个符合消息处理规范的函数(此处为`MessageHandler`代表期望执行的消息处理函数).

当注册的消息发送者`"SKSE"`广播消息, 申请聆听的插件`g_thisPlugin`便会执行其注册的回调函数`MessageHandler`.

函数`RegisterListener`会根据是否成功注册聆听而返回`true`/`false`. 插件作者应以此返回值决定是否继续加载插件.

若函数返回值为`true`, 会打印日志`"Registered messaging interface"`表明注册成功. 若函数返回值为`false`, 会打印日志`"Failed to register messaging interface"`表明注册失败并跳过此次加载.

> 完整示例可以[在这里](/examples/Interfaces/messaging.cpp)查看.

## 插件通信

若期望聆听来自其它插件的消息, 或广播消息至注册的聆听者, 需要聆听者显式声明广播者的插件名称(`a_skse->name`)和符合消息处理规范的回调函数.

以下示例如何为插件项目`PluginA`和插件项目`PluginB`互相注册聆听并打印收到的消息.

注册聆听:
```C++
void MessageHandler(SKSEMessagingInterface::Message* a_msg)
{
    if (a_msg->type == SKSEMessagingInterface::kMessage_PostPostLoad) {
        if (g_messaing->RegisterListener(g_thisPlugin, "PluginB", MyHandler)) {
            _MESSAGE("Registered messaging interface");
        } else {
            _MESSAGE("Failed to register messaging interface");

            return false;
        }
    }
}
```
其中`"PluginB"`为广播者的插件名称(`a_skse->name`), 函数`MyHandler`则是注册的回调函数.

> 为何要在函数`MessageHandler`内注册插件聆听? 因为注册时无法确定广播者的插件是否已完成加载, 因此在收到来自SKSE64的消息`kMessage_PostPostLoad`表明数据和插件均加载完毕后, 再注册插件聆听以确保同步性.

处理消息:
```C++
void MyHandler(SKSEMessagingInterface::Message* a_msg) {
    _MESSAGE(static_cast<const char*>(a_msg));
}
```
函数`MyHandler`负责将收到的消息`a_msg`由类型`Message*`静态类型转换为字符串指针`const char*`使其可以打印.

广播消息:
```C++
char myMessage[16] = "Hello Plugin!";
g_messaging->Dispatch(g_thisPlugin, 0, myMessage, 16, nullptr);
```
函数`Dispatch`: 此函数负责广播消息至特定聆听者或全体聆听者.
参数 | 类型
--- | ---
`PluginHandle` | 广播者插件句柄
`UInt32` | 消息类型(通常为枚举)
`void*` | 消息数据指针
`UInt32` | 消息数据大小
`const char*` | 聆听者插件名称
返回值 | `void`

应总是为`PluginHandle`传入此插件句柄`g_thisPlugin`, 为第一个`UInt32`传入广播者和聆听者约定好的消息辨识枚举(此处未约定, 因此传入0), 为`void*`传入要广播的消息数据的指针(此处为字符串`myMessage`), 为第二个`UInt32`传入数据的大小(字符串`myMessage`的大小为16). 若想广播至所有注册的聆听者, 为`const char*`传入`nullptr`(空指针, 表明不声明特定聆听者). 反之则传入特定聆听者的插件名称.

> 消息辨识枚举: SKSE64广播的消息便使用了消息辨识枚举以表明当前加载阶段(如`kMessage_DataLoaded`).

***
##### [回到上层](/docs/CM/Interfaces.md) | [消息队列](/docs/CM/Interfaces/Messaging.md) | [Papyrus 对象](/docs/CM/Interfaces/Object.md) | [Papyrus 脚本](/docs/CM/Interfaces/Papyrus.md) | [Scaleform 界面](/docs/CM/Interfaces/Scaleform.md) | [序列化](/docs/CM/Interfaces/Serialization.md) | [代理委托](/docs/CM/Interfaces/Task.md)
