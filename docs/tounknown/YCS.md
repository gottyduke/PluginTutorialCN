<h1 align="center">实战案例 <a href="https://www.nexusmods.com/skyrimspecialedition/mods/36057">You Can Sleep - 解除休息/等待限制(重置版)</a></h1>  
<p align="center"><a href="/docs/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数hook</a></p>

难度: &#9733;  
练习耗时: *??*  
[源码](https://github.com/gottyduke/YouCanSleepRemake)

## 0. 思路

YCS的功能: 允许玩家在任何情景下休息/等待.  

0. *这项功能是否可以不用修改内存就能以其他方式做到?*  
应该可以通过某种Papyrus脚本做到(吧?).  
1. *这项功能如果能够以其他方式就达到同样效果, 修改内存相比之下有什么值得选择的优点?*  
内存补丁可以跳过调用游戏的PapyrusVM步骤, 节省时间.  
2. *这项功能的内存补丁应该设置在游戏大概的哪个部分?*  
应当设置在检查休息/等待条件时.  

我们的目标是找到判断当前是否可以休息/等待的函数并为其打上内存补丁. 休息/等待时会调用这个函数判断当前条件是否可以休息/等待, 例如床被占用, 在空中, 在战斗中等.  

## 1. 查找引用

当我们试图在不能休息/等待的情景下休息/等待时, 游戏会提示相关的限制, 这是一个非常好的突破口. 因为游戏必然会先调用休息/等待函数判断条件, 如果不符合条件, 则会从内存中获取符合的字符串构建UI元素.  

打开游戏后在主菜单使用`coc whiterundragonsreach`快速传送到Whiterun Dragonsreach地点, 直奔Farengar Secret-Fire的书房, 他拥有一张床可以为我们提供测试环境. 尝试在他的床上休息会提示我们:  
![ycs_pre_ce](/images/toukn/ycs/re_pre_ce.png)  

附加Cheat Engine至游戏进程上, 并搜索字符串`You cannot sleep in an owned bed.`, 很容易就找到了这个字符串值的内存地址.  
![ycs_ce_owned](/images/toukn/ycs/ce_owned.png)  

在CE中打开查找地址引用窗口并附加调试器后, 在游戏内再次试图休息, CE捕捉到了对这个字符串值的引用:  
![ycs_ce_](/images/toukn/ycs/ce_filter.png)  

首先分析末尾的3条指令, `vmovdqu ymm`(move unaligned double quadword vector)指令是常见的字符串值的向量化优化. 再看第2条和第5条指令, 因为字符串`You cannot sleep in an owned bed.`长度为33, 所以指令2和5也是在处理字符串值. 最后再看第3条和第4条指令, `cmp byte ptr [地址], 0`指令从内存地址中读取了大小为`BYTE`的数据并将其与`0`进行对比(`cmp`, compare), 这是常见的字符串处理每一个字符(`char`的大小为8位, 即`BYTE`)的方式, 一直读取到字符串结尾为`\0`.  

第一条指令`7FF7CADB61E8`:  
```assembly
movsx eax, byte ptr [rdx]
```
这条指令将字符串开头的`BYTE`数据带符号移入了`eax`寄存器(`movsx`, move with sign-extension). C++代码为`char* eax = (char*)(rdx)`.  

将这一条指令的地址记录下来后关闭CE, 下一步更细致的反编译交给x64dbg.  

> 教程的上下文图里地址可能有差异, 因为教程是分开编写的, 每次获取的地址不一样.  
> `byte ptr`限定从内存地址`rdx`读取的数据大小为`BYTE`(8位字节).  

## 2. 断点调试


---
<p align="center"><a href="/docs/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数hook</a></p>
