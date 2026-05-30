#!/bin/bash

# 设置错误处理策略
set -e  # 遇到错误立即退出

echo "=== 开始重新安装 common-utils ==="

# 步骤 1: 移除已安装的包
echo -e "\n[1/3] 正在移除已安装的 common-utils..."
vcpkg remove common-utils:x64-linux --recurse || {
    echo "移除失败，但继续执行清理..."
}

# 步骤 2: 清理 vcpkg 二进制缓存
echo -e "\n[2/3] 正在清理 vcpkg 缓存..."
rm -rf ~/.cache/vcpkg/archives/* 2>/dev/null || true
echo "缓存清理完成"

# 步骤 3: 重新安装
echo -e "\n[3/3] 正在安装 common-utils..."
vcpkg install common-utils --overlay-ports=ports/common-utils --triplet=x64-linux

if [ $? -eq 0 ]; then
    echo -e "\n=== 安装成功完成 ==="
else
    echo -e "\n=== 安装失败，请检查错误信息 ==="
    exit 1
fi