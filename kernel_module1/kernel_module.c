#include <linux/module.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

static int my_kernel_module_init(void)
{
	printk("Init kernel module \n");
	km_proc = proc_create("KernelModule", 0666, NULL, &fproc_ops);
	if(km_proc == NULL) return -1;
	else
	return 0;
}

static void my_kernel_module_exit(void)
{
	printk("Exit kernel moulde \n");
	proc_remove(km_proc);
}

module_init(my_kernel_module_init);
module_exit(my_kernel_module_exit);





















MODULE_LICENSE("GPL");
MODULE_AUTHOR("VANHAO");
MODULE_VERSION("1.0");

