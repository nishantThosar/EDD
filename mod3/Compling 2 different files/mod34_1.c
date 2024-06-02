

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

extern int __init my_mod_init(void);
extern int my_open(struct inode *inode, struct file *file); //open the device
extern ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off); //reading from func
extern ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off); // writing to the file
extern int my_release(struct inode *inode, struct file *file); // closing (releasing)
extern void __exit my_mod_exit(void);

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.write = my_write,
	.release = my_release,
};

extern int my_open(struct inode *inode, struct file *file)
{
	pr_info("My open function %s called\n", __func__);
	return 0;
}

extern ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
	pr_info("My read function %s called\n", __func__);
	return 0;
}

extern ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("My write function %s called\n", __func__);
	return len; //to return the number of bytes written.
}

extern int my_release(struct inode *inode, struct file *file)
{
	pr_info("My release function %s called\n", __func__);
	return 0;
}