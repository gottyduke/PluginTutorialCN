<h1 align="center">调试101</h1>  
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>

本节教程简单讲一下使用x64dbg进行调试(debug)的知识.  

## 断点

断点调试是最常用的调试方式, 当设置的断点目标被击中时, x64dbg会将进程挂起(暂停).  

类型 | 目标 | 击中条件
--- | --- | ---
软件(Software) | 机器指令 | 指令执行至断点处时
硬件(Hardware) | 内存地址 | 读/写访问断点所在的内存地址处时
内存(Memory) | 内存分页 | 读/写访问断点所在的内存分页时
异常(Exception) | 异常类型 | 指定的异常类型被抛出时

## 调试

当断点被击中后, 我们便可以逐步调试程序, 分析每一步指令的作用, 观察寄存器和rflag的数值变化.  

在汇编中, 函数体作为子程序(subroutine)存在于内存里, 通过`call`返程跳转指令调用. 调用者(caller)会先将返程地址入栈再跳转到子程序, 而被调用者(callee)执行完毕后则通过返程指令`ret`返回至栈上储存的地址并将其出栈. 通过返程地址可以找到调用者(caller).  

类型 | 快捷键 | 作用
--- | --- | ---
运行至选区(Run till selection) | `F4` | 执行至当前选中的地址.
步进(Step into) | `F7` | 执行下一条指令, 跟随跳转.
步过(Step over) | `F8` | 执行下一条指令, 不跟随跳转.
运行(Run) | `F9` | 恢复进程运行.
运行至返回(Execute till return) | `Ctrl + F9` | 执行至返程指令`ret`.

x64dbg默认启用TLS回调函数的断点, 我们需要将它禁用掉.  
![debug101_dbg_tls](/images/toukn/debug101_dbg_tls.png)

---
<p align="center"><a href=<p align="center"><a href="/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/resources/Plugin.md">插件基础</a> | <a href="/docs/resources/Papyrus.md">Papyrus调用</a> | <a href="/docs/resources/Events.md">事件响应</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a></p></p>
