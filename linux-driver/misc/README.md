# Linux Misc (杂项) 设备驱动

Misc 设备本质上也是字符设备，但简化了开发流程

* **主设备号固定**：所有的 Misc 设备主设备号都是 **10**。
* **次设备号唯一**：通过不同的次设备号来区分不同的设备。
* **简化流程**：内核自动处理了 `alloc_chrdev_region`、`cdev_init`、`cdev_add` 以及 `class_create` 和 `device_create`。
* **自动创建节点**：只要加载驱动，`/dev/` 下就会自动出现设备节点。

---

## 2. 核心结构体：`struct miscdevice`

定义在 `<linux/miscdevice.h>` 中：

```c
struct miscdevice {
    int minor;                  // 次设备号 (若设为 MISC_DYNAMIC_MINOR 则由内核动态分配)
    const char *name;           // 设备名称 (出现在 /dev/ 目录下的名称)
    const struct file_operations *fops; // 文件操作接口
    struct list_head list;
    struct device *parent;
    struct device *this_device;
    const struct attribute_group **groups;
    const char *nodename;
    umode_t mode;
};
```
