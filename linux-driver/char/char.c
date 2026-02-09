#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "char"
#define BUF_SIZE 1024

static dev_t dev_num;          // 设备号
static struct cdev my_cdev;    // 字符设备结构体
static struct class *my_class; // 设备类
static char kernel_buf[BUF_SIZE]; // 内核缓冲区

// 1. 实现打开函数
static int my_open(struct inode *inode, struct file *file) {
    pr_info("Device opened\n");
    return 0;
}

// 2. 实现读函数 (内核 -> 应用)
static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    int data_len = strlen(kernel_buf);
    pr_info("Device read\n");

    // 1. 检查偏移量：如果已经读过数据了，直接返回 0 (告诉 cat 读完了)
    if (*ppos >= data_len) {
        return 0; 
    }

    // 2. 调整实际读取长度，防止越界
    if (count > data_len - *ppos) {
        count = data_len - *ppos;
    }

    // 3. 拷贝到用户空间
    if (copy_to_user(buf, kernel_buf + *ppos, count)) {
        return -EFAULT;
    }

    // 4. 重要：更新文件偏移量！
    *ppos += count;

    // 5. 返回读取的字节数
    return count;
}

// 3. 实现写函数 (应用 -> 内核)
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    if (copy_from_user(kernel_buf, buf, min(count, (size_t)BUF_SIZE))) {
        return -EFAULT;
    }
    pr_info("Device written: %s\n", kernel_buf);
    return min(count, (size_t)BUF_SIZE);
}

// 4. 实现关闭函数
static int my_release(struct inode *inode, struct file *file) {
    pr_info("Device closed\n");
    return 0;
}

// 5. 绑定操作接口
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release,
};

// 驱动入口
static int __init chrdev_init(void) {
    // A. 动态分配设备号
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    
    // B. 初始化并添加 cdev
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, 1);
    
    // C. 自动创建设备节点 (/dev/char)
    my_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
    
    pr_info("Driver loaded: Major %d\n", MAJOR(dev_num));
    return 0;
}

// 驱动出口
static void __exit chrdev_exit(void) {
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("Driver unloaded\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gemini AI");