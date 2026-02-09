#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/ioport.h>

/* 1. 定义匹配表：对应 DTS 中的 compatible 字符串 */
static const struct of_device_id my_test_of_match[] = {
    { .compatible = "gemini,my-platform-device" },
    { /* 哨兵 */ }
};
MODULE_DEVICE_TABLE(of, my_test_of_match);

/* 2. Probe 函数：硬件与驱动匹配成功后的入口 */
static int my_test_probe(struct platform_device *pdev)
{
    struct resource *res;
    
    dev_info(&pdev->dev, "Probe: Match found via Device Tree!\n");

    /* 提取 reg 资源 */
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "Error: Could not get reg resource from DTS\n");
        return -EINVAL;
    }

    dev_info(&pdev->dev, "DTS Reg Start: 0x%pa, Size: 0x%x\n", 
             &res->start, (unsigned int)resource_size(res));

    return 0;
}

/* 3. Remove 函数：设备移除或驱动卸载 */
static int my_test_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Remove: Driver unlinking from device...\n");
    return 0;
}

/* 4. 定义平台驱动结构体 */
static struct platform_driver my_test_driver = {
    .probe = my_test_probe,
    .remove = my_test_remove,
    .driver = {
        .name = "my_test_platform_driver",
        .of_match_table = my_test_of_match,
    },
};

/* 5. 手动注册入口 (取代宏) */
static int __init my_test_driver_init(void)
{
    int ret;
    pr_info("Initializing and registering to bus...\n");
    
    /* 将驱动注册到 platform 总线 */
    ret = platform_driver_register(&my_test_driver);
    if (ret) {
        pr_err("Failed to register platform driver: %d\n", ret);
        return ret;
    }
    
    return 0;
}

/* 6. 手动注销出口 (取代宏) */
static void __exit my_test_driver_exit(void)
{
    pr_info("Unregistering from bus...\n");
    
    /* 从 platform 总线注销 */
    platform_driver_unregister(&my_test_driver);
}

module_init(my_test_driver_init);
module_exit(my_test_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gemini");