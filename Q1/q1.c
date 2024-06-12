/*Write a Linux loadable kernel module (LKM) that creates character devices after accepting the following kernel module parameters at load time:
1. class=<class-name-input-by-user>
2. device=<device-name-input-by-user>
3. num_devs=<number input by user>

An example invocation of this module could be:
sudo insmod module.ko class=usb device=pendrive num_devs=20

The LKM should then create num_devs devices of the form:
/dev/pendrive00, /dev/pendrive02, ..., /dev/pendrive/19 under the usb class.

Test the LKM thoroughly by loading and unloading 4-5 times, and make sure the kernel remains stable. LKM should clean up all resources it consumes on unload. 
*/

#define pr_fmt(fmt) KBUILD_MODNAME" : "fmt

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

/*-------------------------------------------------------GLOBAL DECLARAIONS---------------------------------------------------------------*/
int dev_number = 1;
char *cls_name  = "Default";
char *device_name = "Def_cls";
dev_t dev = 0;
struct class *class;
struct device *devices[20]; // hardcoded the device array else need to do kmalloc

/*-------------------------------------------------------MODULE PARAM'S---------------------------------------------------------------*/
module_param(cls_name, charp, S_IRUSR | S_IWUSR);
module_param(dev_number, int, S_IRUSR | S_IWUSR);
module_param(device_name,charp, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(cls_name, "This is a char variable for class name");
MODULE_PARM_DESC(device_name, "This is a char variable for device name");
MODULE_PARM_DESC(dev_number, "This is a int variable for number of devices");

/*-------------------------------------------------------INIT FUNCTION---------------------------------------------------------------*/
static int __init Init(void)
{
	int i;
	char _name[10];	
	pr_info("Initializing Devices\n\n");
	if(dev_number == 0) // if user inserts dev_number = 0
	{
		pr_alert("Devices Can't be zero\n");
		return -1;
	}//if ends
	else
	pr_emerg("\nNow only single device will be created in the class"); // printing if user didn't gave any input for dev_number

	if( alloc_chrdev_region(&dev, 0, dev_number, device_name) < 0) //allocating major and minor dynamically
	{
		pr_err("Character Device Error\n");
		return -1;
	}//if ends
	pr_info("Character Devices Created %d \n", MAJOR(dev));

	class = class_create(THIS_MODULE, cls_name); // creating the class
	if(IS_ERR(class))
	{
		pr_err("Class Error\n");
		goto class_error;
	}//if ends
	pr_notice("Class created\n"); // debugger
	//creation of devices as given by user
	for(i=0; i<dev_number; ++i)
	{
		dev = MKDEV(MAJOR(dev), i); // making the device
		sprintf(_name, "%s%d", device_name, i);
		devices[i] = device_create(class, NULL, dev, NULL, _name); 
		if(IS_ERR(devices[i]))
		{
			pr_err("Device Error %d\n", i);
			goto device_error;
		}
		pr_emerg("Major = %d | Minor = %d\n", MAJOR(dev), MINOR(dev));//printing the major and minor
	}
	pr_cont("Total Devices Created = %d\n", dev_number); // to print the total number of devices created
	return 0;
//======================================================== ERROR LADDER =================================================
	device_error:
		class_destroy(class);
	class_error:
		unregister_chrdev_region(dev, 5);
	return -1;
}

/*-------------------------------------------------------EXIT FUNCTION---------------------------------------------------------------*/
static void __exit Exit(void)
{
	int i;//local var
	for(i = 0; i< dev_number; ++i)
	{
		dev = MKDEV(MAJOR(dev), i);
		device_destroy(class, dev);
	}
	class_destroy(class);
	unregister_chrdev_region(dev, dev_number);
	pr_warn("Exiting from module\n");
}

module_init(Init);
module_exit(Exit);
