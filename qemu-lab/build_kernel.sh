#!/bin/bash
set -e

# 设置清华镜像链接
KERNEL_URL="https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.10.10.tar.xz"
KERNEL_TAR="linux-5.10.10.tar.xz"
KERNEL_DIR="linux-5.10.10"
DTS_PATH="arch/arm/boot/dts/vexpress-v2p-ca9.dts"

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-

# 1. 下载逻辑：压缩包不存在才下载
if [ ! -f "$KERNEL_TAR" ]; then
    echo "Downloading kernel from Tsinghua Mirror..."
    wget -c $KERNEL_URL
fi

# 2. 解压逻辑：文件夹不存在才解压
if [ ! -d "$KERNEL_DIR" ]; then
    echo "Extracting kernel..."
    tar -xf $KERNEL_TAR
fi

cd $KERNEL_DIR

# 3. 注入自定义测试节点 (检查是否已存在，防止重复插入)
if ! grep -q "gemini,my-platform-device" "$DTS_PATH"; then
    echo "Injecting custom test node into DTS..."

    sed -i '$i \
    my_test_node {\
        compatible = "gemini,my-platform-device";\
        reg = <0x70000000 0x100>;\
        status = "okay";\
    };' "$DTS_PATH"
fi

# DTS_PATH="arch/arm/boot/dts/vexpress-v2p-ca9.dts"

if ! grep -q "rtc@68" "$DTS_PATH"; then
    echo "Injecting ds1338 into DTS..."
    cat >> "$DTS_PATH" <<EOF

&v2m_i2c_dvi {
    rtc@68 {
        compatible = "ds1338";
        reg = <0x68>;
        status = "okay";
    };
};
EOF
    echo "Done."
fi

# 4. 基础配置
make vexpress_defconfig

# 5. 注入 9P 共享文件夹支持 (驱动开发核心)
scripts/config --enable CONFIG_NET_9P
scripts/config --enable CONFIG_NET_9P_VIRTIO
scripts/config --enable CONFIG_9P_FS
scripts/config --enable CONFIG_9P_FS_POSIX_ACL
scripts/config --enable CONFIG_VIRTIO_MMIO
scripts/config --enable CONFIG_DEBUG_INFO

# I2C
# 开启 I2C 核心支持
./scripts/config --enable CONFIG_I2C
# 开启 I2C 字符设备接口 (即 /dev/i2c-X)
./scripts/config --enable CONFIG_I2C_CHARDEV
# 开启你的控制器支持 (针对 VExpress 平台)
./scripts/config --enable CONFIG_I2C_VERSATILE


# 6. 解决配置依赖并编译
make olddefconfig
make zImage dtbs -j$(nproc)
make modules_prepare

echo "------------------------------------------------"
echo "Build Finished Successfully!"
echo "zImage: arch/arm/boot/zImage"
echo "DTB:    $DTS_PATH -> .dtb"
echo "------------------------------------------------"