# 获取当前 Makefile 所在的绝对路径
COMMON_DIR := $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

# 使用绝对路径包含配置，这样无论在哪里调用 make，路径都不会错
include $(COMMON_DIR)/../env.mk

# 定义模块名
obj-m := misc.o
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
install: all
	cp *.ko $(KO_DIR)