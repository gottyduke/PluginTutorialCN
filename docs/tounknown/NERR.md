<h1 align="center">实战案例 <a href="https://www.nexusmods.com/skyrimspecialedition/mods/34175">No Enchantment Restriction - 解除附魔限制(年度版)</a></h1>  
<p align="center"><a href="/docs/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p>

难度: &#9733;&#9733;  
练习耗时: *~15min*  
[源码](https://github.com/gottyduke/NoEnchantmentRestrictionRemake)

## 0. 思路

当然这里只讨论NERR的内存补丁部分: 修改一件物品允许的**最大附魔数量**.  

0. *这项功能是否可以不用修改内存就能以其他方式做到?*  
可以通过附加自定义perk类似于原版`ExtraEffect`修改允许的附魔数量. 
1. *这项功能如果能够以其他方式就达到同样效果, 修改内存相比之下有什么值得选择的优点?*  
内存补丁可以跳过调用游戏的PapyrusVM步骤, 节省时间.  
2. *这项功能的内存补丁应该设置在游戏大概的哪个部分?*  
应当设置在启动附魔台时.  

我们的目标是找到加载附魔台的函数并为其打上内存补丁. 使用附魔台时一定会从内存中获取当前最大允许的附魔数, 数值由玩家是否拥有`ExtraEffect`perk决定.  

## 1. 查找引用

`是`/`否`(`true`/`false`)在内存中会用(`1`/`0`)表达. 附加Cheat Engine至游戏进程上, 并在游戏里反复添加/移除`ExtraEffect`perk, 当添加perk后, 搜索`1`. 移除perk后, 搜索`0`. 最后确定了指向当前是否有`ExtraEffect`perk的内存地址(以下简称为`perk状态值`):  
![ce_nerr_pre](/images/toukn/nerr/ce_perk_pre.png)  

在CE中打开查找地址引用窗口并附加调试器后, 在游戏内打开附魔台, 果不其然引用了这个perk状态值, 符合我们对游戏加载附魔台函数的猜想:  
![ce_nerr_filter](/images/toukn/nerr/ce_perk_filter.png)  
这里有两条不同的指令, 其中`r8 + rcx * 8 + 10`是perk状态值的内存地址.  

第一条指令`7FF79BF2A6B1`: 
```assembly
cmp dword ptr [r8 + rcx * 8 + 10], 0
```
这条指令将perk状态值与`0`进行比较(`cmp`, compare), C++代码为`if(*(r8+rcx*8+10) == 0)`.  

第二条指令`7FF79BF2A6FA`:
```assembly
mov eax, [r8 + rcx * 8 + 10]
```
这条指令将perk状态值拷贝至`eax`寄存器(`mov`, move), 双击打开这条指令发现后面是`test eax, eax`, C++代码为`if(bool eax = *(r8+rcx*8+10);eax)`.  

将这两条指令的地址记录下来后关闭CE, 下一步更细致的反编译交给x64dbg.  

> 教程的图里地址可能有差异, 因为每次运行时获取的地址可能不一样.  
> perk状态值并非内存中的perk对象.  
> `dword ptr`限定从内存地址`r8 + rcx * 8 + 10`读取的数据大小为`DWORD`(32位双字).  

## 2.1 断点调试

附加x64dbg至游戏进程上, 转到第一条指令的地址`7FF79BF2A6B1`.  
![dbg_nerr_boolcheck](/images/toukn/nerr/dbg_boolcheck.png)  
这个函数加载了第一个参数的第一个成员, 对该成员的成员(偏移量`0x288`)进行null检查并比较第二个参数是否是字符串结尾`\0`, 然后对该成员+偏移量`0x190`进行null检查并返程. 我们将它命名为`sub_check198`.  

因为这个函数内并不包含堆栈帧(stack frame), 因此栈顶就是返程地址:  
![dbg_nerr_ret](/images/toukn/nerr/dbg_ret.png)  

转到返程地址`7FF79BEA1CC5`:  
![dbg_nerr_caller](/images/toukn/nerr/dbg_caller.png)  
在`call`指令后`test al, al`是返回值null检查. 这个函数将第一个参数的成员(偏移量`0xF0`)传递给函数`sub_check198`后对返回值进行null检查并返程. 我们将它命名为`sub_check288`.  
> 也可以手动步进跟随指令返程而不直接跳转到返程地址.  

这个调用者函数很小, 明显不是加载附魔台的主函数, 需要返回到更上一层的调用者函数. 因为包含堆栈帧(stack frame), 所以需要加上`0x28`的栈偏移才是返程地址. 随着指令执行, 我们来到了更上一层的调用者函数.  
![dbg_nerr_2ndcaller](/images/toukn/nerr/dbg_2ndcaller.png)  
挺长的一个函数, 截屏都没有截完整. 由于返程在这个函数主体的中间部分, 因此我们在其头部`test rdx, rdx`打上断点并进入游戏测试. 这里可以吃惊(吃惊吗?)的发现在没有打开附魔台时, 这个断点就被立刻触发了, 说明这个函数也不是加载附魔台的主函数, 仅仅是其中的一个调用.  

根据CE查找到的引用信息, 附魔台打开时只调用了一次perk状态值, 而此处的函数明显是在游戏循环里反复调用(继续运行会立刻击中下一次断点), 结合我们的上一个函数`sub_check288`的返回值经常变化(说明参数经常变化), 可以合理猜想这是用于循环检测玩家是否有perk(常见于Papyrus脚本中). 我们为它命名为`sub_checkPerk`.  

## 2.2. 断点调试

已经知道函数`sub_checkPerk`是处于一个循环中, 那我们就无法在设置软件断点后返回游戏打开附魔台了, 因为断点会立刻击中. 这里就需要设置一个硬件断点, 当函数`sub_check198`的参数为我们想要的`ExtraEffect`时, 挂起程序. 

在x64dbg的内存视图中转到perk状态值的地址, 并为其设置硬件断点, 因为函数`sub_check198`调用这个地址时是`dword`, 因此硬件断点的条件也为读取dword(32位双字)时.  
![dbg_nerr_hardbp](/images/toukn/nerr/dbg_hardbp.png)  
恢复游戏运行后在游戏内打开附魔台, 击中了硬件断点后再次回到x64dbg.  

一直步进到函数`sub_checkPerk`, 因为明确知道这个函数只是附魔台加载函数中的一个调用, 所以我们可以执行至返程指令`ret`前. 再次步进后可以看见我们来到了一个非常大的调用者函数(假装没看见我的注释):  
![dbg_nerr_loadmain_src](/images/toukn/nerr/dbg_loadmain_src.png)  
看见x64dbg已经把此时各个寄存器的值给解析了出来, 其中附魔相关的字符串`Enchanting`, `Choose an item to destroy ...`都证明了这就算不是附魔台加载的主函数, 也是附魔台相关的调用.  

从函数`sub_checkPerk(ExtraEffect)`返程后, 我们看见了许多和浮点相关的指令, 从`movss xmm1, [rbp+588]`开始, 到`cvttss2si rax, xmm0`结束. 这一串指令用于浮点数整型转换. 结合游戏Form中perk附加的数值都是浮点数来看, 这一段指令的意义就不言而喻了: 调用函数`sub_checkPerk(ExtraEffect)`检测玩家是否有`ExtraEffect`perk, 根据返回值加载perk数值的浮点数, 将浮点数整型转换并拷贝至`rax`寄存器用于后面的调用.  

这后面的指令就和perk无关了: `rax`中的整型值被拷贝至`r14`, 一个用于构建UI的字符串指针被拷贝至`rax`后再移入一个本地变量等UI相关的操作. 此时我们已经找到了内存补丁的目标: 从函数`sub_checkPerk`返回后将我们想要的值拷贝至`rax`寄存器.  

## 3. 内存补丁

既然我们的目标是将想要的值拷贝至`rax`寄存器, 那么这一片浮点数整型转换的操作就不需要了. 在x64dbg中选中这一片内存, 将其以无操作指令`NOP`填充:  
![dbg_nerr_nops](/images/toukn/nerr/dbg_nops.png)  

选中第一个`NOP`, 按下空格键输入汇编`mov eax, 5`后恢复游戏运行:  
![nerr_done](/images/toukn/nerr/re_done.png)  
就是这样简单的一个汇编指令, 我们就改变了游戏允许的最大附魔数量 - 无论有无`ExtraEffect`perk, 无论这个perk被魔改成什么样.  
> 这里用`eax`而不是`rax`因为我们的值是32位常量, 所以目的寄存器也限定为32位.  

## 4. SKSE

现在我们需要将这个操作复现在SKSE插件中. x64dbg中向上定位到这个函数的头部并复制它的相对偏移地址(RVA):  
![dbg_nerr_rva](/images/toukn/nerr/dbg_rva.png)  

根据RVA在当前版本的Address Library中找到对应的ID:  
![re_nerr_id](/images/toukn/nerr/re_id.png)  

x64dbg中双击函数头部的地址切换为偏移量模式, 并回到我们编写内存补丁的地方获取偏移量:  
![dbg_nerr_offset](/images/toukn/nerr/dbg_offset.png)  
其中`0x212`是补丁入口, `0x243`是补丁出口.  

C++代码:  
```C++
#include "DKUtil/Hook.hpp"

using namespace DKUtil::Alias;

// 1-6-323: 0x894EE0 + 0x212
constexpr std::uint64_t FuncID = 51242;
constexpr std::ptrdiff_t OffsetLow = 0x212;
constexpr std::ptrdiff_t OffsetHigh = 0x243;

constexpr OpCode AsmSrc[]{
    0xB8,					// mov eax,
    0x00, 0x00, 0x00, 0x00, // Imm32
};

constexpr std::ptrdiff_t ImmediateOffset = sizeof(OpCode);

HookHandle _Hook_UES;


void Install()
{
    constexpr Patch AsmPatch = {
        std::addressof(AsmSrc),
        sizeof(AsmSrc)
    };

    _Hook_UES = DKUtil::Hook::AddASMPatch<OffsetLow, OffsetHigh>(DKUtil::Hook::IDToAbs(FuncID), &AsmPatch);
    _Hook_UES->Enable();

    DKUtil::Hook::WriteImm(_Hook_UES->TramPtr + ImmediateOffset, static_cast<Imm32>(5));

    INFO("Hooks installed"sv);
}
```
网上有非常多的资源可以在线转换汇编指令为对应的机器码, 为我们省下时间. 但是如果想要掌握反编译的能力, 对于汇编的进一步理解是不可少的.  

我们的目标是`mov r32, imm32`, 将源32位常量值拷贝至目的32位寄存器(前缀`e`), 对应的`mov`指令为`0xB8`, 目的寄存器`ax`机器码为`0`, 所以指令为`0xB8(mov) + 0(eax), 0x00000000(32位常量占位符)`即`0xB8, 0x00000000`.  

通过DKUtil写入汇编后, 我们再写入想要的最大附魔数. `mov eax, 32位常量`汇编指令中`mov eax`指令(`0xB8`)大小为1个字节, 所以数值应该写在`函数地址+补丁偏移+1字节指令偏移`处. DKUtil的`ASMHandle`类成员`TramPtr`会指向`函数地址+补丁偏移`, 我们直接在它的值加上指令偏移.  

启动游戏后再次打开x64dbg并转到附魔台加载函数的地址. 如果之前没有注释, 可以看看log.  
![re_nerr_log](/images/toukn/nerr/re_log.png)  
x64dbg中查看插件修改成果:  
![dbg_nerr_doneAGAIN](/images/toukn/nerr/dbg_doneAGAIN.png)  
因为剩下的无操作`NOP`指令很多, DKUtil自动添加了一个跳转到下一条指令以提升性能.  

最后游戏里测试没有问题, 这个实战案例就到此结束了:))

## 5. FAQ

Q: 为什么不直接设置硬件断点, 省略前面的步骤?  
A: 作为上帝视角, 的确可以这样操作, 但在实际反汇编理解游戏函数逻辑的过程中, 这些都是一步一步摸索出来的. 没有前面设置软件断点反复调试的经验, 哪怕一来就设置硬件断点, 也缺少对这几个函数的理解, 反而会影响效率.  

Q: 为什么特别版和年度版的反汇编不一样?  
A: 因为B社升级了用于编译游戏的MSVC版本, 新版MSVC对很多函数做了优化/折叠处理.  

Q: 这个案例很难练习, 很多指令看不懂!  
A: 汇编指令都是其名字的缩写, 多查多记.  

Q: 案例看完了, 下一步干什么?  
A: [函数Hook](/docs/tounknown/FuncHook.md)

---
<p align="center"><a href="/docs/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p>
