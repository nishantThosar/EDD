/* Illustrate module parameters without callbacks */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt // tp print the module name in dmesg

#include <linux/module.h>
#include <linux/moduleparam.h> // for parameter support
#include <linux/init.h>

// instantiate the parameter variables with some defaults
// these will be overriden by insmod values, if any
static int myint = 100; // creates int var with name myint
static char *mycharp = "Hello";// creates char ptr var with name mycharp
#define SIZE	(10)
static long myarr[SIZE] = {0};
static int count = 0; // will contain how many array entries were received

// register them with the kernel
// perms: all can read, only USR (root) can write/edit
module_param(myint, int, S_IRUGO | S_IWUSR); // to enable parameters in kernal
module_param(mycharp, charp, S_IRUGO | S_IWUSR); // to enable char var in kernal 
module_param_array(myarr, long, &count, S_IRUGO | S_IWUSR); // to enable array in kernal

// create MODULE DESC strings for modinfo
MODULE_PARM_DESC(myint, "This is an integer variable");
MODULE_PARM_DESC(mycharp, "This is a char pointer variable");
MODULE_PARM_DESC(myarr, "This is an array variable of longs");//module para descriptor

static int __init my_mod_init(void)
{
	int i = 0;
	pr_info("Hello from mod21!\n");
	pr_info("myint = %d, mycharp=%s\n", myint, mycharp);
	pr_info("For myarr, we got %d entries\n", count);
	for (;i<count;i++)
	{
		pr_info("myarr[%d]= %ld ", i, myarr[i]); //to print the input values
	}
	return 0;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye from mod21!\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module with param support!");
