# linux-driver/env.mk

# 1. 交叉编译变量
ARCH := arm
CROSS_COMPILE := arm-linux-gnueabi-

# 2. 内核源码路径 (相对于各子目录的通用路径，或使用绝对路径)
# 建议使用绝对路径，最稳妥
KO_DIR := ../../qemu-lab/shared_dir/
KDIR := ../../qemu-lab/linux-5.10.10/

# 3. 导出变量给子 Makefile 使用
export ARCH CROSS_COMPILE KDIR
