$Header = '<a href="/docs/README.md">回到目录</a> | <a href="/docs/setup/Setup.md">工具配置</a> | <a href="/docs/setup/Script.md">脚本说明</a> | <a href="/docs/setup/QuickStart.md">快速入门</a> | <a href="/docs/tounknown/MemPatch.md">内存补丁</a> | <a href="/docs/tounknown/FuncHook.md">函数Hook</a>'

$Docs = Get-ChildItem $PSScriptRoot -File -Filter *.md -Recurse

foreach ($doc in $Docs) {
    $content = [IO.File]::ReadAllText($doc)
    
    $content = $content -replace '(?s)(?:(?<=\<p align\="center"\>)(.*?)(?=</p\>))', $Header
    
    [IO.File]::WriteAllText($doc, $content)

    "Formatted $($doc.Name)"
}    