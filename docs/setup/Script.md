<h1 align="center">脚本说明</h1>
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>

示例工作项目包含三个常用脚本辅助开发, `!Rebuild`, `!MakeNew`, 和`!Update`.  

---
+ ## `!Rebuild <编译库|BOOTSTRAP> <游戏版本> [自定义CLib]`
用于重新生成整个解决方案.  
参数 | 说明
--- | ---
`BOOTSTRAP` | 设置相应的系统环境变量以及工具链.
`编译库` | `MT`(静态编译`MultiThreaded`)或`MD`(动态编译`MultiThreadedDLL`), 使用的`vcpkg`为`x64-windows-static`或`x64-windows-static-md`. 无特殊需求建议使用`MT`参数.
`游戏版本` | `AE`(天际年度版, `1.6.xxx`)或`SE`(天际特别版, `1.5.97`).
`自定义CLib` | 可选参数`0`, 启用自定义CLib代替默认的CLib作为开发库. 不使用自定义CLib时无视此选项.  
`无参数` | 同步对项目做出的更改(添加/删除文件等). 更新VS项目`ZERO_CHECK`.  

---
+ ## `!MakeNew <项目名称> [-install: mod名称] [-message: 项目说明] [-vcpkg: 额外依赖项]`  
用于快速新建符合此工作项目规格的插件项目.  
参数 | 说明
--- | ---
`项目名称` | 项目名称, 同步于`CMakeLists.txt`, `vcpkg.json`和解决方案内.
`mod名称` | `-install`或`-i`, 指定MO2安装时的mod名称(默认为项目名称).
`项目说明` | `-message`或`-m`, 简易的项目说明, 生成于附属的`vcpkg.json`文件内. 用双引号`"`包含.
`额外依赖项` | `-vcpkg`或`-v`, 额外的`vcpkg`依赖项. 默认为`spdlog`. 多个依赖项用逗号分隔. 

---
+ ## `!Update <运行模式>`  
用于编译后复制文件, 为`CMakeLists.txt`生成源文件表, 以及自动更新. 在正常使用此工作项目时, 此脚本会被自动更新到每一个下属项目中并嵌入至生成的解决方案中, 通常不需要单独运行.  
参数 | 说明
--- | ---
`COPY` | 由VS编译后事件调用, 需求多个参数. 
`DISTRIBUTE` | 由`!Rebuild`脚本调用, 发布本脚本的最新版至每一个下属的插件项目文件夹中.
`SOURCEGEN` | 由CMake调用, 生成当前项目的源文件表.  

---
<p align="center"><a href="./README.md">回到目录</a> | <a href="./docs/setup/Setup.md">工具配置</a> | <a href="./docs/setup/Script.md">脚本说明</a> | <a href="./docs/tounknown/FuncHook.md">函数hook</a> | <a href="./docs/tounknown/MemPatch.md">内存补丁</a> | <a href="./docs/QuickStart.md">快速入门</a></p>
