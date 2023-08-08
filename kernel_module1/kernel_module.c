#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>


static int my_kernel_module_init(void)
{
	printk("Init kernel module \n");
	return 0;
}

static void my_kernel_module_exit(void)
{
	printk("Exit kernel moulde \n");
}

module_init(my_kernel_module_init);
module_exit(my_kernel_module_exit);





















MODULE_LICENSE("GPL");
MODULE_AUTHOR("VANHAO");
MODULE_VERSION("1.0");

