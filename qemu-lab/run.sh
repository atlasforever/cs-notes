#!/bin/bash

# 确保共享文件夹存在
mkdir -p ./shared_dir

qemu-system-arm -M vexpress-a9 \
    -m 512M \
    -kernel linux-5.10.10/arch/arm/boot/zImage \
    -dtb linux-5.10.10/arch/arm/boot/dts/vexpress-v2p-ca9.dtb \
    -initrd rootfs.img.gz \
    -append "console=ttyAMA0 root=/dev/ram rdinit=/sbin/init" \
    -nographic \
    -fsdev local,security_model=none,id=fsdev0,path=./shared_dir \
    -device virtio-9p-device,fsdev=fsdev0,mount_tag=hostshare
