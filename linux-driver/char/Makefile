obj-m := dummy_char.o

ARCH=arm
CROSS_COMPILE=arm-anycloud-linux-uclibcgnueabi-
KDIR=/home/zhaozj/works/AnyCloud39AV130_SDK_V1.01/os/kbd

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean