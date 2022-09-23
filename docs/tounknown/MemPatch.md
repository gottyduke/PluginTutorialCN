<h1 align="center">内存补丁</h1>  
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

## 概念

内存补丁顾名思义在游戏内存内打上我们期望的补丁. 补丁可以是一个跳转, 一个修改后的数据, 或者直接改动机器指令. 在[Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444)的加持下, 为上古卷轴5: 天际打内存补丁格外容易.

## 需求

在涉及任何内存补丁的操作后, 代码通常不再安全, 不再稳定, 不再容易追溯bug.  
在为插件项目引入内存补丁前, 问自己三个问题:  

0. 这项功能是否可以不修改内存, 以其他方式做到?  
1. 这项功能如果能够以其他方式达到同样效果, 修改内存相比之下有什么值得选择的优点?  
2. 这项功能的内存补丁应该设置在游戏大概的哪个部分?  

## 流程

0. 查找引用
1. 定位函数
2. 反汇编
3. 代码复现

## 可读资料

[Dropkicker的汇编101](/docs/tounknown/ASM101.md)  
[Dropkicker的调试101](/docs/tounknown/DEBUG101.md)  

## 实战案例

### [You Can Sleep - 解除休息/等待限制(特别版)](/docs/tounknown/YCS.md) &#10003;
难度: &#9733;  

### [No Enchantment Restriction - 解除附魔限制(年度版)](/docs/tounknown/NERR.md) &#10003;
难度: &#9733;&#9733;  

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
