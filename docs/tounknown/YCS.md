<h1 align="center">实战案例 <a href="https://www.nexusmods.com/skyrimspecialedition/mods/36057">You Can Sleep - 解除休息/等待限制(重置版)</a></h1>  
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

难度: &#9733;  
练习耗时: *~15mins*  
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
![re_ycs_pre_ce](/images/toukn/ycs/re_pre_ce.png)  

附加Cheat Engine至游戏进程上, 并搜索字符串`You cannot sleep in an owned bed.`, 很容易就找到了这个字符串值的内存地址.  
![re_ycs_ce_owned](/images/toukn/ycs/ce_owned.png)  

在CE中打开查找地址引用窗口并附加调试器后, 在游戏内再次试图休息, CE捕捉到了对这个字符串值的引用:  
![ycs_ce_](/images/toukn/ycs/ce_filter.png)  

首先分析末尾的3条指令, `vmovdqu ymm`(move unaligned double quadword vector)指令是常见的字符串值的向量化优化. 再看第2条和第5条指令, 因为字符串`You cannot sleep in an owned bed.`长度为33, 所以指令2和5也是在处理字符串值. 最后再看第3条和第4条指令, `cmp byte ptr [地址], 0`指令从内存地址中读取了大小为`BYTE`的数据并将其与`0`进行比较(`cmp`, compare), 这是常见的字符串处理每一个字符(`char`, 大小为8位, 即`BYTE`)的方式, 一直读取到字符串结尾为`\0`.  

第一条指令`7FF7CADB61E8`:  
```assembly
movsx eax, byte ptr [rdx]
```
这条指令将字符串开头的`BYTE`数据带符号拷贝至`eax`寄存器(`movsx`, move with sign-extension). C++代码为`char* eax = (char*)(rdx)`.  

将这一条指令的地址记录下来后关闭CE, 下一步更细致的反编译交给x64dbg.  

> 教程的图里地址可能有差异, 因为每次运行时获取的地址可能不一样.  
> `byte ptr`限定从寄存器`rdx`内存地址中读取的数据大小为`BYTE`(8位字节).  

## 2.1 断点调试

附加x64dbg至游戏进程上, 转到第一条指令的地址`7FF7CADB61E8`, 为其设置软件断点.  
![dbg_ycs_strptr](/images/toukn/ycs/dbg_strptr.png)  
这个函数从上级调用者传递的参数`rdx`中读取了一个大小为`BYTE`的数据, 并将此数据拷贝至`eax`寄存器以作他用. 这是将字符串指针所指向的字符串首字符作为内存地址传递, 类似于C++中的`&buffer[0]`. 我们将它命名为`sub_loadString`.  

为函数`sub_loadString`设置断点后, 这个断点就被立刻触发了. 此时我们并未在游戏中试图在占用的床上休息, 在x64dbg中也可以注意到各个寄存器的值都是随机的, 大多为游戏内对于AI事件的调用名, 这通常来自于Papyrus脚本.  

## 2.2 断点调试

既然函数`sub_loadString`被反复调用, 那我们就无法在打软件断点后返回游戏测试了, 因为断点会立刻触发. 这里就需要设置一个硬件断点, 当函数`sub_loadString`的参数为我们想要的`You cannot sleep in an owned bed.`字符串时, 触发断点. 

在x64dbg的内存视图中转到字符串的地址, 并为其打上硬件断点, 因为函数`sub_loadString`调用这个地址时是`byte`, 因此硬件断点的条件也为读取byte(8位字节)时.  
![dbg_ycs_hardbp](/images/toukn/ycs/dbg_hardbp.png)  
恢复游戏运行后在游戏内再次试图休息, 击中了硬件断点后回到x64dbg. 此时记得取消硬件断点以免调试过程中无法步过(step over).  

![dbg_ycs_hardbp](/images/toukn/ycs/dbg_hbphit.png)  
因为知道这个函数只是用于加载字符串并对其进行字符串相关的操作(具体操作并没有分析, 但可以看见下方的`call <&toupper>`), 因此我们步进到上级调用者函数(`Ctrl+F9`).

第一个调用者函数:  
![dbg_ycs_1stcaller](/images/toukn/ycs/dbg_1stcaller.png)  
可以看见这个函数依然不是我们的目标, 这是一个用于构建UI元素的函数, 这一点可以从寄存器值`UIMenuCancel`看出. 为了跳过这个UI函数的各个调用/跳转部分, 我们直接在函数体末尾的返程指令`ret`处设置软件断点并恢复游戏进程运行, 随后步进一次来到第二个调用者函数体.  

第二个调用者函数(部分):  
![dbg_ycs_call](/images/toukn/ycs/dbg_call.png)  
通过x64dbg解析出的各个值, 可以确定这个函数便是我们的目标. 这一部分的逻辑非常简单, 在这个函数里判断是否能够休息/等待, 如果失败, 便加载相应的UI字符串并调用下级函数发送UI信息. 游戏里如果可以休息/等待时, 会有一个UI面板询问休息/等待多长游戏时间, 而我们此次测试环境中是不能休息的(床被占用), 因此函数发送了`UIMenuCancel`来取消显示询问UI, 并发送相应的信息字符串至另一个UI函数作为提示显示给玩家, 即`You cannot sleep in an owned bed.`.  

第二个调用者函数(主体):  
![dbg_ycs_main](/images/toukn/ycs/dbg_main.png)  

回到目标函数的主体来分析逻辑, 我们可以发现, 各种条件判断都是这样一个逻辑: 先调用一个判断该条件的子函数, 再根据返回值, 成功便跳转到下一个条件判断, 失败则加载相应的提示信息字符串, 并跳转到此函数末尾处的UI调用相关部分以发送提示信息. 结合示例来看:  
![dbg_ycs_cond](/images/toukn/ycs/dbg_cond.png)  
首先是调用子函数判断条件`call skyrimse.7FF6C66357C0`, 随后测试返回值`test al, al`, 成功便执行跳转开始下一个条件判断(trespassing)`je`, 失败则加载字符串`mov rcx, 字符串指针地址`, 随后跳转到函数末尾`jmp`.  

## 3. 内存补丁

目标函数已经找到, 它的逻辑我们也分析过了, 每一个子条件判断中, 成功则跳转, 失败则加载UI字符串再跳转. 我们要做的就是让游戏认为我们的每一个条件都是成功的, 因此永远不会失败跳转. 这个内存补丁的实施方法太多了, 比如将调用子条件函数的指令替换为我们的函数, 比如将测试返回值的指令改为永远为`1`等等. 在这里YCS插件选择的是将跳转指令从条件跳转`je/jne`(jump if/not zero-flag)替换为无条件跳转`jmp`(unconditional jump).

根据我们分析的逻辑(`call`-`test`-`je/jne`-`字符串`-`jmp`), 很快在函数靠近末尾处(偏移量`0x3BC`)找到了此次测试所使用的条件判断"床是否被占用"(`You cannot sleep in an owned bed.`):  
![dbg_ycs_owned_jmp](/images/toukn/ycs/dbg_owned_jmp.png)  
选中条件跳转指令`jne skyrimse.7FF6C66CD687`, 按下空格将汇编`jne`替换为`jmp`后返回游戏并再次测试:  
![re_ycs_succeeded](/images/toukn/ycs/re_succeeded.png)  
非常简单的指令替换, 我们便解除了休息/等待其中之一的子条件`owned`.  

接下来我们将此函数中的总计8个子条件跳转指令全部替换并再次进入游戏详细测试, 此时会发现一个问题: 其他的子条件补丁都生效了, 但是在空中时依然不能休息/等待.  
![re_ycs_noair](/images/toukn/ycs/re_noair.png)  

回到条件判断函数主体, 通过设置断点和单步调试的方式可以发现此处有一个循环:  
![dbg_ycs_air_loop](/images/toukn/ycs/dbg_air_loop.png)  
这个循环其实并不难理解, 当玩家在空中时, 每一帧滞空都属于`in air`因此会有一个循环判断. 我们要做的就是跳过这个循环, 将形成循环的跳转指令`jmp`替换为无操作指令`nop`.  
![dbg_ycs_air_loop_nop](/images/toukn/ycs/dbg_airloop_nop.png)  
再次测试发现子条件`in air`也被成功跳过了.  

> 函数最末尾的`You cannot sleep at this time`并不符合我们的逻辑(`call`-`test`-`je/jne`-`字符串`-`jmp`), 可以忽略.  

## 4. SKSE

现在我们需要将这个操作复现在SKSE插件中. x64dbg中向上定位到这个函数的头部并复制它的相对偏移地址(RVA):  
![dbg_ycs_rva](/images/toukn/ycs/dbg_rva.png)  

根据RVA在当前版本的Address Library中找到对应的ID:  
![re_ycs_id](/images/toukn/ycs/re_id.png)  

x64dbg中双击函数头部的地址切换为偏移量模式, 并记录下8个子条件内存补丁的偏移量:  
```
0x2E
0x89
0xB1
0xF6
0x11F
0x146
0x1BB
0x3BC
```
以及子条件`in air`的循环偏移量`0xD4`.  

C++代码:  
```C++
#include "DKUtil/Hook.hpp"

using namespace DKUtil::Alias;

constexpr OpCode JmpShort = 0xEB;
constexpr OpCode NOP = 0x90;

// 1-5-97-0 0x69D2C0
constexpr std::uint64_t FuncID = 39371;
constexpr std::ptrdiff_t OffsetTbl[8]{
    0x2E,	// You cannot sleep in the air.
    0x89,	// You cannot sleep while trespassing.
    0xB1,	// You cannot sleep while being asked to leave.
    0xF6,	// You cannot sleep while guards are pursuing you.
    0x11F,	// You cannot sleep when enemies are nearby.
    0x146,	// You cannot sleep while taking health damage.
    0x1BB,	// This object is already in use by someone else.
    0x3BC	// You cannot sleep in an owned bed.
};

constexpr std::ptrdiff_t InAirLoopOffset = 0xD4;

OpCode InAirLoopNop[6]{ NOP, NOP, NOP, NOP, NOP, NOP };

void Install()
{
    const auto funcAddr = DKUtil::Hook::IDToAbs(FuncID);

    for (auto index = 0; index < std::extent_v<decltype(OffsetTbl)>; ++index) {
        DKUtil::Hook::WriteImm(funcAddr + OffsetTbl[index], JmpShort);
    }

    // loop check for in air position
    DKUtil::Hook::WriteData(funcAddr + InAirLoopOffset, &InAirLoopNop, sizeof(InAirLoopNop));

    INFO("Hooks installed"sv);
}
```

最后游戏里测试没有问题, 这个实战案例就到此结束了:))

## 5. FAQ

Q: 这个案例很难练习, 很多指令看不懂!  
A: 汇编指令都是其名字的缩写, 多查多记.  

Q: 案例看完了, 下一步干什么?  
A: [实战案例#2](/docs/tounknown/NERR.md)

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
