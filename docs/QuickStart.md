<h1 align="center">快速入门</h1>
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>

本教程将会指引你以最简短的操作步骤编译出一个可在 _上古卷轴5 AE - 1.6.xx_ 版本的游戏中运行的**SKSE插件**.  
参照此教程进行操作之前, 请先确保已按照[开发环境](/docs/setup/Setup.md)文档中的要求安装配置好所需的前置开发环境.
<br/><br/>

---  
## 安装开发项目库
1. 以管理员身份启动PowerShell.  
   
   ![启动PS](/images/quickstart/1.png)  
<br/>

2. 重定向至合适的文件路径, 并输入以下指令clone开发用项目库到对应的文件路径中.(教程中所选用的路径为"`F:\SKSE_Development`")  
    ```ps
    git clone https://github.com/gottyduke/SKSEPlugins
    ```  
<br/>

3. 设置执行策略以运行脚本：使用以下指令来设置执行策略以获得运行PS脚本的权限.(执行策略修改只会在本次终端运行期间生效)  
    ```ps
    Set-ExecutionPolicy Bypass -Scope Process
    ```
    在输出的执行选项指令中输入`Y`以执行修改.  

    ![修改执行策略](/images/quickstart/3.png)  
<br/>  

4. 跳转到`SKSEPlugins`目录并执行`BOOTSTRAP`以安装前置项目库和依赖项.
    ```ps
    cd .\SKSEPlugins
    .\!Rebuild BOOTSTRAP
    ```
<br/>

5. 等候安装程序的运行, 直到看到弹出`custom CLib Support`选项窗口.  
   该选项允许你设置自定义的CommonLibSSE项目库路径, 本教程里先暂时忽略掉它, 点击`否`以继续执行安装程序.  

    ![自定义Clib框](/images/quickstart/5.png)  
<br/>

6. 跳过SE版1.5.97的设置, 随后依照安装程序指引分别设置好：AE版游戏根目录路径、AE版游戏的MO2文件路径.(MO2文件路径可以不进行设置)

    ![AE根目录](/images/quickstart/6.png)  

    ![AE的MO2路径](/images/quickstart/7.png)  

<br/>

7. 设置作者名字.

    ![设置作者名字](/images/quickstart/8.png) 
<br/>

8. 当看到` Restart current command line interface to complete BOOTSTRAP.`消息后, 表明BOOTSTRAP过程已经完成.

    ![BOOTSTRAP完成](/images/quickstart/9.png)   

---
## 重启PowerShell

__完成上面的操作后, 请务必关闭PowerShell并重新打开! 否则会造成接下来的指令报错不能运行! ! !__  

__完成上面的操作后, 请务必关闭PowerShell并重新打开! 否则会造成接下来的指令报错不能运行! ! !__   

__完成上面的操作后, 请务必关闭PowerShell并重新打开! 否则会造成接下来的指令报错不能运行! ! !__
<br/> 

---
## 新建插件源代码项目
1. 重新打开PowerShell, 定位到`\SKSEPlugins`文件夹后, 输入以下指令以新建一个名为"MyFirstPlugin"的SKSE插件项目.
    ```PS
    .\!MakeNew MyFirstPlugin
    ```
    ![MakeNew](/images/quickstart/10.png)  
<br/> 

2.  随后便可在"`\SKSEPlugins\Plugins\MyFirstPlugin`"文件目录中找到新建项目的源代码文件.

    ![NewPluginPath](/images/quickstart/11.png)   
<br/>   

3. 关于`!MakeNew`脚本的具体使用说明可以查阅[脚本说明](/docs/setup/Script.md).

---
## 生成工程项目
源代码项目已经建立好了, 接下来我们需要生成visual studio的工程项目.  

1. 在PowerShell中接着输入以下指令, 以生成一个用于开发*Multithreaded Runtime*的AE版SKSE插件工程项目.
    ```PS
    .\!Rebuild MT AE
    ```
    ![BuildProject](/images/quickstart/12.png)
<br/>

2. 生成成功后,  "MyFirstPlugin"的VS工程项目便可在"`SKSEPlugins\Build\Plugins\MyFirstPlugin`"文件夹下所找到.
   
   ![ProjectSolution](/images/quickstart/13.png)
<br/>

3. 关于`!Rebuild`脚本的具体使用说明可以查阅[脚本说明](/docs/setup/Script.md).

---
## 编译DLL插件
1. 打开`SKSEPlugins\Build\Plugins\MyFirstPlugin`路径下的`MyFirstPlugin.sln`项目, 然后点击生成项目解决方案以编译DLL插件, 编译插件成功后的输出结果如下图所示.
   
    ![BuildOutput](/images/quickstart/14.png)

<br/>

2. 随后便可在"`SKSEPlugins\Build\bin\Debug`"路径下找到生成的`MyFirstPlugin.dll`插件.

    ![PluginPath](/images/quickstart/15.png)
   
<br/>

3. 若在BOOTSTRAP阶段时已经设置了MO2文件路径, 则一个新的名为`MyFirstPlugin`的mod会出现在MO2的MOD列表中(若没有则刷新一下MO2), `MyFirstPlugin.dll`则已被拷贝到此mod文件里面.
    
    ![MO2Plugin](/images/quickstart/16.png)

<br/>

4. 若未设置MO2文件路径, 则`MyFirstPlugin.dll`会被拷贝到AE版游戏根目录下的"`Data\SKSE\Plugins`"路径下.

---
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>
