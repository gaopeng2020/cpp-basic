# 设置错误处理策略
$ErrorActionPreference = "Stop"

Write-Host "=== 开始重新安装 common-utils ===" -ForegroundColor Cyan

# 步骤 1: 移除已安装的包
Write-Host "`n[1/3] 正在移除已安装的 common-utils..." -ForegroundColor Yellow
vcpkg remove common-utils:x64-mingw-dynamic --recurse
if ($LASTEXITCODE -ne 0) {
    Write-Host "移除失败，但继续执行清理..." -ForegroundColor Red
}

# 步骤 2: 清理 vcpkg 二进制缓存
Write-Host "`n[2/3] 正在清理 vcpkg 缓存..." -ForegroundColor Yellow
Remove-Item -Recurse -Force $env:LOCALAPPDATA\vcpkg\archives\* -ErrorAction SilentlyContinue
Write-Host "缓存清理完成" -ForegroundColor Green

# 步骤 3: 重新安装
Write-Host "`n[3/3] 正在安装 common-utils..." -ForegroundColor Yellow
vcpkg install common-utils --overlay-ports=ports/common-utils --triplet=x64-mingw-dynamic

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n=== 安装成功完成 ===" -ForegroundColor Green
} else {
    Write-Host "`n=== 安装失败，请检查错误信息 ===" -ForegroundColor Red
    exit $LASTEXITCODE
}
