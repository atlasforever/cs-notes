# 字符设备

## 1. 概念

字符设备（Character Device）是指必须以**字节**为单位进行顺序读写的设备（如串口、LED、各种传感器）。

### 1.1 设备号 (Device Number)

在 Linux 中，每个设备都有一个唯一的设备号，由**主设备号**和**次设备号**组成：

* **主设备号 (Major)**：标识设备对应的驱动程序。
* **次设备号 (Minor)**：标识使用同一驱动程序的具体某个设备实例。
* **类型定义**：`dev_t`（实质是 32 位无符号整数，高 12 位为主设备号，低 20 位为次设备号）。

### 1.2 关键结构体

1. `struct file_operations`：定义了应用层 `open`、`read`、`write` 等系统调用在内核中的具体实现。
2. `struct cdev`：表示一个字符设备的内核对象。
3. `struct inode`：内核中每个文件对应一个 `inode`，内部含有 `i_cdev` 指针。
4. `struct file`：代表一个打开的文件，内部含有 `f_op` 指针。

---

## 2. 基本流程

1. **申请主设备号**
    * 手动（`register_chrdev_region`）：已知确定的主设备号，且未占用
    * 动态（`alloc_chrdev_region`）：自动找一个空闲的主设备号，推荐
2. **初始化 cdev**：关联 `file_operations`。
3. **添加 cdev**：将设备加入内核（`cdev_add`）。
4. **自动创建设备节点**：使用 `class_create` 和 `device_create`，省去手动 `mknod`。
5. **实现回调函数**：编写 `read`、`write`、`open`、`release` 的具体逻辑。
