#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/bcd.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>

#define DEVICE_NAME "rtc"
#define RTC_REG_SIZE 7

struct my_rtc_dev {
    struct i2c_client *client;
    struct cdev cdev;
    dev_t devid;
};

static struct class *rtc_class;
static struct my_rtc_dev *rtc_devp;

static ssize_t rtc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct i2c_client *client = file->private_data;
    u8 data[RTC_REG_SIZE];
    char out_str[64];
    int len, ret;
    int sec, min, hour, day, mon, year;

    ret = i2c_smbus_read_i2c_block_data(client, 0x00, RTC_REG_SIZE, data);
    if (ret < 0) return -EIO;

    sec  = bcd2bin(data[0] & 0x7F);
    min  = bcd2bin(data[1]);
    hour = bcd2bin(data[2] & 0x3F);
    day  = bcd2bin(data[4]);
    mon  = bcd2bin(data[5]);
    year = bcd2bin(data[6]) + 2000;

    len = sprintf(out_str, "RTC Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                  year, mon, day, hour, min, sec);

    if (*ppos > 0) return 0;
    if (copy_to_user(buf, out_str, len)) return -EFAULT;
    
    *ppos += len;
    return len;
}

static int rtc_open(struct inode *inode, struct file *file)
{
    struct my_rtc_dev *dev = container_of(inode->i_cdev, struct my_rtc_dev, cdev);
    file->private_data = dev->client;
    return 0;
}

static const struct file_operations rtc_fops = {
    .owner = THIS_MODULE,
    .open  = rtc_open,
    .read  = rtc_read,
};

static int rtc_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    dev_t devid;
    
    /* * 注意：使用设备树匹配时，id 指针可能为 NULL (取决于内核版本)，
     * 但现在的内核核心层通常会在 device_id 表中查找匹配项并传进来。
     * 如果你需要获取 device tree 中的特定数据，可以使用:
     * const struct of_device_id *match = of_match_device(rtc_of_match, &client->dev);
     */

    dev_info(&client->dev, "Probing via %s\n", client->dev.of_node ? "Device Tree" : "Legacy Board Info");

    rtc_devp = devm_kzalloc(&client->dev, sizeof(struct my_rtc_dev), GFP_KERNEL);
    if (!rtc_devp) return -ENOMEM;

    rtc_devp->client = client;
    alloc_chrdev_region(&devid, 0, 1, DEVICE_NAME);
    rtc_devp->devid = devid;

    cdev_init(&rtc_devp->cdev, &rtc_fops);
    cdev_add(&rtc_devp->cdev, devid, 1);

    rtc_class = class_create(THIS_MODULE, "rtc_class");
    device_create(rtc_class, NULL, devid, NULL, DEVICE_NAME);

    dev_info(&client->dev, "My RTC Driver Probed: /dev/%s created\n", DEVICE_NAME);
    return 0;
}

static int rtc_remove(struct i2c_client *client)
{
    device_destroy(rtc_class, rtc_devp->devid);
    class_destroy(rtc_class);
    cdev_del(&rtc_devp->cdev);
    unregister_chrdev_region(rtc_devp->devid, 1);
    return 0;
}

/* 1. 定义设备树匹配表 (OF Match Table) */
static const struct of_device_id rtc_of_match[] = {
    { .compatible = "dallas,ds1338" },
    { .compatible = "dallas,ds1307" },
    { /* Sentinel */ }
};
/* 2. 导出设备树表，以便模块自动加载 */
MODULE_DEVICE_TABLE(of, rtc_of_match);

static struct i2c_driver rtc_driver = {
    .driver = { 
        .name = "my_rtc_driver",
        .of_match_table = rtc_of_match, // 3. 将 of_match_table 注册进驱动结构
    },
    .probe = rtc_probe,
    .remove = rtc_remove,
};

module_i2c_driver(rtc_driver);
MODULE_LICENSE("GPL");