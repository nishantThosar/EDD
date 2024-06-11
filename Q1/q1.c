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

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
/*============================================MACROS============================================================*/
#define MY_DEV_NAME	"EDD"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD ");
MODULE_DESCRIPTION("LKM that creates character devices!");
MODULE_PARM_DESC(myint, "This is an integer variable");
/*=========================================GLOBAL DECLARATIONS==================================================*/
static char string[] = "\tMy module"; // just module name
static int my_var = 1; // var for model_param
dev_t dev = 0; // var to hold major & minor value
module_param(my_var,	int, S_IRUGO | S_IWUSR);//for accepting input
/*============================================INIT FUNCTION=====================================================*/
static int __init MAIN(void)
{
	int ans; // for catching the return value of alloc chardev region
	pr_cont("Hello from %s!\n", string);
	ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
	if (ans<0)
	{
		pr_alert("Error in major:minor allotment!\n");
		return -1;
	}//if ends

	pr_info("\tMajor:%d Minor:%d allotted!\n", MAJOR(dev),MINOR(dev));
	pr_info("\td Value:%d\n", my_var);
	return 0;
}//__inint ends

/*==========================================EXIT FUNCTION=======================================================*/
static void __exit EXIT(void)
{

	unregister_chrdev_region(dev, 1);
	pr_info("major:%d minor:%d numbers freed up...\n",MAJOR(dev),MINOR(dev));
	pr_alert("Goodbye world!\n");
	return;
}//__exit ends

module_init(MAIN);
module_exit(EXIT);

