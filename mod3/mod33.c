/* Automatic device node creation */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#define MY_CLASS_NAME	"MY_CLASS" // class name necessary for kernel.
#define MY_DEV_NAME	"MY_CLASS_NAME" // name of device name which will access this driver //for kernel under the "cdac_cls"
#define NUM_DEVS	(5) //no of devices

dev_t dev = 0; //init of dev var with 0;
static struct class *dev_class; // struct pointer of class (file descriptor)
static struct device *cdevice[NUM_DEVS]; // arr of struct to struct device (file descriptor) 

static int __init my_mod_init(void)
{
	int ans,i, major, minor;
	char dev_string[10]={0};

	pr_info("Hello world from mod33!\n");
	ans = alloc_chrdev_region(&dev, 0, NUM_DEVS, MY_DEV_NAME); //asking 5 devices to kernal & pass a generic name as last paramerer.
	/*This region will contain one major number and 5 minor numbers as 5 devices are created*/
	if (ans<0) // if device(major & minor) are not generated 
	{
		pr_err("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor range of %d devices %d:%d allotted!\n", NUM_DEVS, MAJOR(dev),MINOR(dev)); //to print major & minor range of 5 devices.

	dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);//to create class for 5 devices
	if (IS_ERR(dev_class)) // IS_ERR=> kernal macro to check if there is error(as it is returning class(struct pointer) not 1 or 0).
	{
		pr_err("Could not create device class %s\n", MY_CLASS_NAME);
		goto r_class; // if class is not created then go to r_class 
	}

	major = MAJOR(dev); // major device
	for (i=0;i<NUM_DEVS;i++) //to create 5 devices
	{
		minor = i; // so to start from 0

		dev = MKDEV(major, minor); // to make 5 devices with different minor
		// sprintf(dev_string, "%s%c", MY_DEV_NAME, i); //
		sprintf(dev_string, "%s%c", MY_DEV_NAME, i+65);
		cdevice[i] = device_create(dev_class, NULL, dev, NULL, dev_string);
		if (IS_ERR(cdevice[i]))
		{
			pr_err("Could not create device %s\n", dev_string);
			goto r_device;
		}
	}
	pr_info("%d devices %s under class %s created with success\n", 
			i, MY_DEV_NAME, MY_CLASS_NAME);
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;

/*	r_device:
	for (j = 0; j < i; ++j)
	{
		minor = j;
		dev = MKDEV(major,minor);
		device_destroy(dev_class,dev);
	}
	class_destroy(dev_class);

//if class creation fails
r_class:
	minor = 0;
	dev = MKDEV(major,minor);
	unregister_chrdev_region(dev, 5);
	return -1;*/

}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from mod33!\n");
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	pr_info("major:minor numbers freed up...\n");
	return;
}

/*static void __exit my_mod_exit(void)
{
	int i = 0;
	int major, minor;
	pr_info("Goodbye world from class!\n");

	major = MAJOR(dev);
	for (i = 0; i < NUM_DEVS; ++i)
	{
		minor = i; // so to start from 0
		dev = MKDEV(major, minor);
		device_destroy(dev_class, dev);
	}
		class_destroy(dev_class);
		unregister_chrdev_region(dev, 5);
	pr_info("major:minor numbers freed up...\n");
	return;
}*/

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Device node creation module!");
