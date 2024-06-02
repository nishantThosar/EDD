

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

#define MY_CLASS_NAME	"cdac_cls"
#define MY_DEV_NAME	"cdac_dev"

dev_t dev = 0;
static struct class *dev_class; // ptr
static struct device *cdevice; // ptr
static struct cdev my_cdev; //var

/*The func prototypes are extern means no other module can access these modules*/
// function prototypes
extern int __init my_mod_init(void);
extern int my_open(struct inode *inode, struct file *file); //open the device
extern ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off); //reading from func
extern ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off); // writing to the file
extern int my_release(struct inode *inode, struct file *file); // closing (releasing)
extern void __exit my_mod_exit(void);

extern struct file_operations fops;

extern int __init my_mod_init(void)
{
	int ans;

	pr_info("Hello world from mod34!\n");
	ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
	if (ans<0)
	{
		pr_err("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));

	// initialize a cdev
	cdev_init(&my_cdev, &fops);//initilization of cdev & copies fops in it

	ans = cdev_add(&my_cdev, dev, 1);
	if (ans<0)
	{
		pr_err("Could not add cdev to the kernel!\n");
		goto r_cdev;
	}

	dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
	if (IS_ERR(dev_class))
	{
		pr_err("Could not create device class %s\n", MY_CLASS_NAME);
		goto r_class;
	}

	cdevice = device_create(dev_class, NULL, dev, NULL, MY_DEV_NAME);
	if (IS_ERR(cdevice))
	{
		pr_err("Could not create device %s\n", MY_DEV_NAME);
		goto r_device;
	}
	pr_info("Device %s under class %s created with success\n", 
			MY_DEV_NAME, MY_CLASS_NAME); //if everything is successful then this will be printed
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	cdev_del(&my_cdev);
r_cdev:
	unregister_chrdev_region(dev, 1);

	return -1;
}

extern void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from mod34!\n");
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("File operations on char device module!");


