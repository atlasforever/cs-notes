#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

// 1. 实现文件操作接口 (与标准字符设备完全一致)
static int my_misc_open(struct inode *inode, struct file *file) {
    pr_info("Misc device opened\n");
    return 0;
}

static ssize_t my_misc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    pr_info("Misc device read\n");
    return 0; // 返回0防止cat刷新
}

static const struct file_operations my_misc_fops = {
    .owner = THIS_MODULE,
    .open = my_misc_open,
    .read = my_misc_read,
};

// 2. 定义并初始化 miscdevice 结构体
static struct miscdevice my_misc_dev = {
    .minor = MISC_DYNAMIC_MINOR, // 动态分配次设备号
    .name = "misc",       // 设备节点将出现在 /dev/misc
    .fops = &my_misc_fops,       // 绑定操作接口
};

// 3. 驱动入口
static int __init my_misc_init(void) {
    // 只需要这一行，就完成了设备号申请、cdev初始化和节点创建
    int ret = misc_register(&my_misc_dev);
    if (ret) {
        pr_err("Failed to register misc device\n");
        return ret;
    }
    pr_info("Misc driver loaded with minor %d\n", my_misc_dev.minor);
    return 0;
}

// 4. 驱动出口
static void __exit my_misc_exit(void) {
    // 对应销毁
    misc_deregister(&my_misc_dev);
    pr_info("Misc driver unloaded\n");
}

module_init(my_misc_init);
module_exit(my_misc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gemini AI");