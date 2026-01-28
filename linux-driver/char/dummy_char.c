#include <linux/module.h>
#include <linux/init.h>

static int __init dummy_init(void)
{
    pr_info("dummy char driver init\n");
    return 0;
}

static void __exit dummy_exit(void)
{
    pr_info("dummy char driver exit\n");
}

module_init(dummy_init);
module_exit(dummy_exit);