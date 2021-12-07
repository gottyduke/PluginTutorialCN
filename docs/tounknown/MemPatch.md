<h1 align="center">内存补丁</h1>  
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>

> 这一部分作者太懒了, 直接拿自己的几个项目和当时的思路举例.

---
<h2 align="center">内存补丁</h2>

+ ### 概念
内存补丁顾名思义在游戏内存内打上我们期望的补丁. 补丁可以是一个跳转, 一个修改后的数据, 或者直接改动机器指令. 在[地址库](https://www.nexusmods.com/skyrimspecialedition/mods/32444)的加持下, 为天际特别版打内存补丁格外容易.

+ ### 流程
在涉及任何内存补丁的操作后, 代码通常不再安全, 不再稳定, 不再容易追溯bug.  
在为插件项目引入内存补丁前, 问自己三个问题:  
1. 这项功能是否可以不用修改内存就能以其他方式做到?  
2. 这项功能如果能够以其他方式就达到同样效果, 修改内存相比之下有什么值得选择的优点?  
3. 这项功能的内存补丁应该设置在游戏大概的哪个部分?  

+ ### 实战案例(AE)
[No Enchantment Restriction Remake - 解除附魔限制](https://www.nexusmods.com/skyrimspecialedition/mods/34175)  
当然这里只讨论NERR的内存补丁部分: 修改一件物品允许的附魔数量.  
流程三个问题: 
1. 可以通过附加自定义perk类似于原版`ExtraEffect`修改允许的附魔数量. 
2. 内存补丁可以跳过调用游戏的PapyrusVM步骤, 节省时间.  
3. 应当设置在启动附魔台时.  

确定了思路后那么首先需要找到启动附魔台调用允许的附魔数量的函数.  
使用xEdit新建一个记录, 覆盖原版的`ExtraEffect`的数值为`10.0`并保存.  
![EF_Override](/images/toukn/ef_override.png)  
加载游戏后为人物添加新的perk`ExtraEffect`(58f7f). 此时允许的附魔数量应当为`10`.  
![EF_Applied](/images/toukn/ef_applied.png)
附加Cheat Engine至游戏进程上, 并通过控制当前附加的附魔数量来搜索当前附魔数的指针.  
![CE_PreFilter](/images/toukn/ce_prefilter.png)
最后确定了两个值都指向当前附魔数, 初步猜想为位于栈上的本地变量和最大附魔数的实际引用.  
在CE中附加调试器后开始采集这两个值的引用信息.  


---
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>
