/* Major:Minor static allottment */

/*dev_t is a 32 bit quantity
12 bits for major number & 20 bits for minor number*/


#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define MY_MAJOR_NUM	(202)
#define MY_DEV_NAME	"cdac_edd"
//We don't need to give this in dynamic allocation of major & minor numbers
dev_t dev = MKDEV(MY_MAJOR_NUM, 0);  // this is requesting major number as 202 & minor number as 0.
//202 (major number) is then passed in line no 21 through "dev" var. 
static int __init main(void)
{
	int ans;

	pr_info("Hello world from mod31!\n");
	ans = register_chrdev_region(dev, 1, MY_DEV_NAME); // here kernel tries to assign 202(major) & 0(minor) for a device.
	if (ans<0)
	{
		pr_info("Error in major:minor allotment!\n");
		return -1;
	}
	pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));
	return 0;
}

static void __exit exit(void)
{
	pr_info("Goodbye world from mod31!\n");
	unregister_chrdev_region(dev, 1); // as we have registered the major & minor numbers we need to unregister them too.
	pr_info("major:minor numbers freed up...\n");
	return;
}

module_init(main);
module_exit(exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("major:minor static allotment module!");
