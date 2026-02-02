#!/bin/bash
set -e

# 准备目录
mkdir -p rootfs_root/dev rootfs_root/proc rootfs_root/sys rootfs_root/etc/init.d rootfs_root/mnt/shared
cp -a busybox-1.35.0/_install/* rootfs_root/

# 创建启动脚本
cat <<EOF > rootfs_root/etc/init.d/rcS
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
/sbin/mdev -s
# 自动挂载宿主机共享目录
mount -t 9p -o trans=virtio hostshare /mnt/shared
echo "--- QEMU ARM32 Lab Started ---"
EOF

chmod +x rootfs_root/etc/init.d/rcS

# 制作 cpio 镜像
cd rootfs_root
find . | cpio -o --format=newc | gzip > ../rootfs.img.gz
cd ..

echo "Rootfs Image Generated: rootfs.img.gz"
