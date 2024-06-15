/* Write an LKM that implements 2 commands as IOCTLs:
1. START_DEVICE
2. STOP_DEVICE

The device is actually a kernel thread. So START_DEVICE should start a kernel thread that prints to dmesg every 1 second. STOP_DEVICE should stop the kernel thread.

Failure cases like user stopping a device which has not been started, and also user forgetting to stop the device and unloading the module should be handled.

The module should load and unload cleanly. Test the module thoroughly and upload results.
*/

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/fs.h> // for major & minor
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h> //for char device
#include <linux/ioctl.h> //for IOCTL
#include <linux/kthread.h> // for thread
#include <linux/delay.h>//for msleep

#include "my_ioctls.h" // user defined header file

#define MY_CLASS_NAME	"cdac_cls"
#define MY_DEV_NAME	"cdac_dev"
#define TIME_INTVL	(1000)	// milliseconds
/*---------------------------------------------------- GLOBAL DECLARATIONS -------------------------------------------------------*/
dev_t dev = 0; 
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;
static struct task_struct *thread1 = NULL; // thread var
static char thread1_name[] = "Thread-1"; // thread name
static int FLAG = 0;
/*------------------------------------------------------ FUNCTION PTOROTYPES ----------------------------------------------------*/
static int __init my_mod_init(void);
module_init(my_mod_init);
static void __exit my_mod_exit(void);
module_exit(my_mod_exit);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
/*---------------------------------------------------- FUNCTION DECLARATIONS -------------------------------------------------------*/
//Thread function
int thread_func1(void *pv)
{
	int i = 0;
	while(!kthread_should_stop())
	{
		pr_info("In thread %s, i=%d\n", __func__, i++);
		msleep(TIME_INTVL);
	}//while ends
 	return 0;
}// thread function ends

//open function just to print the open
static int my_open(struct inode *inode, struct file *file)
{
	pr_info("My open function %s called\n", __func__);
	return 0;
}//MY OPEN ENDS
//release function just to print release
static int my_release(struct inode *inode, struct file *file)
{
	pr_info("My release function %s called\n", __func__);
	return 0;
}//release function ends
/*-------------------------------------------------- IOCTL FUNCTION ----------------------------------------------------------------*/
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int value;	
	pr_info("Function %s called\n", __func__);
	switch(cmd)
	{
		case START_DEVICE:
			// if ( copy_from_user( &value, (unsigned int *)arg, sizeof(value)) )
			// {
			// 	pr_err("Error reading from kernel variable\n");
			// }

			// if thread is already made then no need to make a thread
			if (thread1)
			{
				pr_err("Thread already made\n");
				break;
			}// if ends
			// //Thread creation
			// if (value ==1)
			// {
			thread1 = kthread_run(thread_func1, NULL, thread1_name);
			if (thread1)
				pr_info("Thread %s created!\n", thread1_name);
			else
			{
				pr_err("Cannot create thread %s\n", thread1_name);
			}
			pr_info("Kernel variable read, value = %d\n", value);				
			// }//if ends
			
			break;
		case STOP_DEVICE:
			// if ( copy_to_user((unsigned int *)arg, &value, sizeof(value)) )
			// {
			// 	pr_err("Error reading from kernel variable\n");
			// }
			// if (value == 2)
			// {
			pr_info("Stopping order received\n");
			kthread_stop(thread1); // STOP SIGNAL FOR THREAD
			FLAG =1; // setting the flag
			// }// if ends
			break;
		default:
			pr_err("Undefined command!\n");
			break;
	}
	return 0;
}// my ioctl ends

// create a fops struct
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.unlocked_ioctl = my_ioctl,
	.release = my_release,
};

/*------------------------------------------------------------------------ INIT FUNCTOIN ******************************************************************************/
static int __init my_mod_init(void)
{
	int ans;

	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
	if (ans<0)
	{
		pr_err("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));

	// initialize a cdev
	cdev_init(&my_cdev, &fops);

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
			MY_DEV_NAME, MY_CLASS_NAME);
	return 0;
// ERROR LADDER
r_device:
        class_destroy(dev_class);
r_class:
        cdev_del(&my_cdev);
r_cdev:
        unregister_chrdev_region(dev, 1);

	return -1;
}// init ends

static void __exit my_mod_exit(void)
{
	if (FLAG != 1)
	{
		kthread_stop(thread1);
	}// if ends
	device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);

	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);
	return;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo time differencing!");