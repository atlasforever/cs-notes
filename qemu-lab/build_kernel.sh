#!/bin/bash
set -e

# 设置清华镜像链接
KERNEL_URL="https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.10.10.tar.xz"
KERNEL_DIR="linux-5.10.10"

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

# 1. 下载与解压
if [ ! -d "$KERNEL_DIR" ]; then
    echo "Downloading kernel from Tsinghua Mirror..."
    wget -c $KERNEL_URL
    tar -xvf linux-5.10.10.tar.xz
fi

cd $KERNEL_DIR

# 2. 基础配置
make vexpress_defconfig

# 3. 注入 9P 共享文件夹支持 (驱动开发核心)
scripts/config --enable CONFIG_NET_9P
scripts/config --enable CONFIG_NET_9P_VIRTIO
scripts/config --enable CONFIG_9P_FS
scripts/config --enable CONFIG_9P_FS_POSIX_ACL
scripts/config --enable CONFIG_VIRTIO_MMIO
scripts/config --enable CONFIG_DEBUG_INFO

# 4. 解决配置依赖并编译
make olddefconfig
make zImage dtbs -j$(nproc)
make modules_prepare
echo "Kernel Build Finished!"
