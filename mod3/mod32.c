/* Major:Minor dynamic allottment */
/*Here due to func "alloc_chardev_region" the dynamic allocation of major & minor is possible*/
#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define MY_DEV_NAME	"cdac_edd"
dev_t dev = 0; // just take a var and assign a value to it

static int __init main(void)
{
	int ans; // temp var to catch the return val of func.

	pr_info("Hello world from mod32!\n"); // debugger
	ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
	//fail check
	if (ans<0)
	{
		pr_info("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev)); // print the dynamically allocated major and minor numbers
	return 0; // if sucess then return 0.
}

static void __exit bye(void)
{
	pr_info("Goodbye world from mod32!\n");
	unregister_chrdev_region(dev, 1); // to unregister the driver made
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(main);
module_exit(bye);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("major:minor dynamic allotment module!");
