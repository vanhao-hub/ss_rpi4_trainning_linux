#include <linux/module.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

static struct proc_dir_entry *km_proc = NULL;
static bool read_completed = false;
static int file_open(struct inode *device,struct file *instance)
{
	printk("File open was called \n");
	return 0;
}

static int file_close(struct inode *device, struct file *instance)
{
	printk("File close wasl called\n");
	return 0;
}

static ssize_t file_read(struct file *File, char *user_buffer, size_t size, loff_t *offs)
{
	if (read_completed) {
        return 0; 
    }
    read_completed = true;
	return (copy_to_user(user_buffer, "Hello user\n",11)?0:11);
}

static ssize_t file_write(struct file *File, const char *user_buffer, size_t size, loff_t *offs)
{
	char user_data[10];
	memset(user_data, 0, 10);
	if(copy_from_user(user_data, user_buffer, size))
		return 0;
	printk("user write : %s \n", user_data);
	return size;
};

static const struct proc_ops fproc_ops = {
	.proc_open	= file_open,
	.proc_read	= file_read,
	.proc_release	= file_close,
	.proc_write	= file_write,
};

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

