#!/bin/bash
# 安装必要工具
sudo apt update
sudo apt install -y qemu-system-arm gcc-arm-linux-gnueabi build-essential \
                    libncurses-dev flex bison libssl-dev libelf-dev cpio

# 设置交叉编译环境变量 (仅当前窗口有效)
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

echo "Environment ready. ARCH=$ARCH, CROSS_COMPILE=$CROSS_COMPILE"
