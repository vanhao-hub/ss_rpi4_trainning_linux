#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/io.h>

static volatile uint32_t *BASE;
static volatile uint32_t *GPFSEL1;
static volatile uint32_t *GPSET0; 
static volatile uint32_t *GPCLR0; 


static struct proc_dir_entry *km_proc = NULL;
static bool read_completed = false;
static int file_open(struct inode *device,struct file *instance)
{
	printk("[vanhao_dbg]File open was called \n");
	return 0;
}

static int file_close(struct inode *device, struct file *instance)
{
	printk("[vanhao_dbg]File close was called \n");
	return 0;
}

static ssize_t file_read(struct file *File, char *user_buffer, size_t size, loff_t *offs)
{
	if (read_completed) {
        return 0; 
    }
    read_completed = true;
	return (copy_to_user(user_buffer, "[vanhao_dbg]Hello user \n",11)?0:11);
}

static ssize_t file_write(struct file *File, const char *user_buffer, size_t size, loff_t *offs)
{
	char user_data[10];
	memset(user_data, 0, 10);
	int value;
	if(copy_from_user(user_data, user_buffer, size))
		return 0;
	kstrtoint(user_data, 10, &value);
	switch (value)
	{
		case 1:
			*GPSET0 = (1 << 18); 
			printk("[vanhao_dbg]gpio18 dat o muc cao \n");
			break;
		case 2:
			*GPCLR0 = (1 << 18); 
			printk("[vanhao_dbg]gpio18 keo xuong muc thapt \n");
			break;
		default:
			break;
	}
	printk("[vanhao_dbg]user write : %s \n", user_data);
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
	// mapping dia chi vat ly sang dia chi cua kernel space
	BASE = (uint32_t *)ioremap(0xFE200000, 256); 

    GPFSEL1 = BASE + (0x04 / 4); 
    GPSET0 = BASE + (0x1C / 4); 
    GPCLR0 = BASE + (0x28 / 4); 

	// GPFSEL1 Register: chon chuc nang cho gpio18
    *GPFSEL1 &= ~(0x7 << 24);
    *GPFSEL1 |= (1 << 24);

    *GPSET0 = (1 << 18); // gpio18 dat o muc cao

	km_proc = proc_create("KernelModule", 0666, NULL, &fproc_ops);
	if(km_proc == NULL) return -1;
	else
	printk("[vanhao_dbg]Init kernel module \n");
	return 0;
}

static void my_kernel_module_exit(void)
{
	*GPCLR0 = (1 << 18); // gpio18 keo xuong muc thap
    iounmap(BASE); // giai phong vung nho duoc mapping ban dau
	proc_remove(km_proc);
	printk("[vanhao_dbg]Exit kernel moulde \n");
}

module_init(my_kernel_module_init);
module_exit(my_kernel_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VANHAO");
MODULE_VERSION("1.0");

