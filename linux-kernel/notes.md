# Introduction

Linux 源码目录介绍如 Lec1 PDF 所示。

# Booting

## 传统 x86-BIOS 系统启动
![](images\21.png)

1. 启动，CPU 跳至 BIOS
2. BIOS 初始化硬件，从 MBR （Master Boot Record）中加载执行 bootloader stage 1
3. stage 1 会加载完整的 bootloader，bootloader 会有各种复杂功能（如 GRUB）
4. bootloader 加载 OS

## 嵌入式系统启动

1. 启动，CPU 跳至 bootloader（如 U-Boot）
2. 初始化硬件
3. 加载 OS

### Device Tree

传统方式中，kernel 包含完整的硬件信息，具体的当前硬件信息可能要 bootloader 通过 ATAGS （U-Boot 下称为 bootargs）告知 kernel

使用 Device Tree 后，bootloader 会把 DTB （Device Tree Blob）二进制内容传递给 kernel，DTB 中直接包含着具体的硬件信息（见 `arch/arm/boot/dts`） 

## 内核

* vmlinux：ELF 格式的内核
* Image：通过 objcopy 生成的纯二进制的内核镜像
* zImage, bzImage：压缩过后的内核镜像，头部有解压缩代码
* uImage：U-Boot 使用的压缩内核镜像，在 zImage 前头加上部分信息

执行流程：

1. head.S：解压缩、初始化处理器、内存、页表等
2. init/main.c：启动 kernel，解析 bootloader 传递的 cmdline，进行各种初始化

## 用户空间初始化

最常见的 System V Init 流程：

* /sbin/init 为一号进程，所有用户空间进程的祖先
* init 读取配置文件 /etc/inittab
* init 在进入或退出某一 run-level 时，会执行对应的 /etc/rc.d 下的脚本
  * run-level 3：/etc/rc3.d，多用户命令行界面
  * run-level 5：/etc/rc5.d，图形界面

# 进程

## 进程与线程

对于 Linux 内核而言线程与进程没什么区别，线程只是能共享资源的进程。

* **kernel thread**
  * 只运行在内核空间的进程，用于完成某些系统任务
  * 如普通进程一样被调度器调用

## 描述符与结构

* **`task_struct`：表示进程的结构体，包含其所有信息。由 slab 分配于内核空间中**

  * `state`：进程状态
  * `thread_info`：进程的底层信息
  * `fs`：当前目录
  * `mm`：用于内存管理
  * ……

* **进程状态**

  * `TASK_RUNNING`：可以运行（准备运行与正在运行）
  * `TASK_INTERRUPTIBLE`：挂起（睡眠），不能运行，待信号唤醒
  * `TASK_UNINTERRUPTIBLE`：同上，但不可被信号唤醒
  * `TASK_STOPPED`：停止
  * `TASK_TRACED`：调试态
  * `EXIT_ZOMBIE`：僵尸进程，待父进程回收
  * `EXIT_DEAD`

## 创建与销毁

* **进程创建**
  * copy-on-write：子进程只读方式复制父进程页表，以共享物理页。写入时产生 page-fault 异常，再申请新的物理页。
  * `fork()`最终调用 `do_fork()`
* **进程销毁**
  * 最终调用`do_exit()`
  * 僵尸进程会占用`struct thread_info`、`struct task_struct`、内核栈等内存空间

# 进程调度

## 策略

* **优先级**
  * 优先级高的先运行，同样优先级的轮转
  * 非实时策略下，使用`nice`（值 20 到 -19）作为优先级
  * 实时调度策略下，使用 `sched_priority`（值 1 到 99）作为优先级
  * 实时策略进程的优先级永远大于非实时进程
* **时间片**
  * 分时复用
  * 使用`nice`权重计算时间片长度
  * 太长影响并发，太短频繁切换影响性能

## 算法

* 三种 scheduling classes：
  * `SCHED_FIFO`：实时策略，一直运行直到阻塞或主动释放 CPU 所有权
  * `SCHED_RR`：实时策略，加上时间片的`SCHED_FIFO`
  * `SCHED_NORMAL`：使用 CFS 的公平调度
* 完全公平调度 CFS
  * 保持进程调度的公平性
  * 用红黑树存储`TASK_RUNNABLE`的进程，以对 CPU 的需求程度进行排序



# 中断

* **中断处理函数**
  * 不应做长耗时行为
  * 处于中断上下文
  * 驱动程序可使用`request_irq()`注册处理函数
  * 使用`soft IRQ`或`tasklet`来延后中断的耗时操作
* **中断上下文**
  * 独立的栈
  * 与进程无关
  * 不能进行 sleep （与进程无关，调度器无法寻回原处）

# 内核同步

* **内核抢占 Kernel Preemption**

  * 进程处于内核态执行内核函数时，可以被另一进程抢占
  * 防止进程长时间卡在某些设备驱动或系统调用中
  * 

  

