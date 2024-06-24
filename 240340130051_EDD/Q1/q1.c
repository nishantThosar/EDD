/*In this question while loading the module the kernel log will print Hey You!
	At unloading in kernel log it will show  Logging out!*/

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>



static int __init my_mod_init(void)
{
	pr_info("Hey You!\n");
	return 0;
}//__inint ends

static void __exit my_mod_exit(void)
{
	pr_alert("Logging out!\n");
	return;
}//__exit ends

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("N");
MODULE_DESCRIPTION("Basic module!");
