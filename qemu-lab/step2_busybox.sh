#!/bin/bash
set -e

BB_URL="https://busybox.net/downloads/busybox-1.35.0.tar.bz2"
BB_DIR="busybox-1.35.0"

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

if [ ! -d "$BB_DIR" ]; then
    echo "Downloading BusyBox from Tsinghua Mirror..."
    wget -c $BB_URL
    tar -xjf busybox-1.35.0.tar.bz2
fi

cd $BB_DIR
make defconfig
# 开启静态编译，防止运行报错
sed -i 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/' .config
make install -j$(nproc)

echo "BusyBox Build Finished!"
