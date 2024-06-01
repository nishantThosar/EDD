/* Illustrate module parameters without callbacks */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt // tp print the module name in dmesg

#include <linux/module.h>
#include <linux/moduleparam.h> // for parameter support
#include <linux/init.h>

// instantiate the parameter variables with some defaults
// these will be overriden by insmod values, if any
static int myops = 1; // creates int var with name myint (Default is addition)
static int myint1 = 50;// creates int var with name myint
static int myint2 = 70;// creates int var with name myint


// register them with the kernel
// perms: all can read, only USR (root) can write/edit
module_param(myops, int, S_IRUGO | S_IWUSR); // to enable parameters in kernal
module_param(myint1, int, S_IRUGO | S_IWUSR); // to enable char var in kernal
module_param(myint2, int, S_IRUGO | S_IWUSR); // to enable char var in kernal 
// module_param_array(myarr, long, &count, S_IRUGO | S_IWUSR); // to enable array in kernal

// create MODULE DESC strings for modinfo
MODULE_PARM_DESC(myops, "This is an operation variable");
MODULE_PARM_DESC(myint1, "This is a int pointer variable");
MODULE_PARM_DESC(myint2, "This is a int pointer variable");


static int __init my_mod_init(void)
{

	
	pr_info("myops = %d, myint1=%d\n", myops, myint1);
	// pr_info("For myarr, we got %d entries\n", count);

	if (myops == 1)
	{
		pr_alert("Addition: %d\n",myint1+myint2);
	}
	else if (myops == 2)
	{
		pr_alert("Subtraction: %d\n",myint1-myint2);
	}
	else if (myops == 3)
	{
		pr_alert("Multiplication: %d\n",myint1*myint2);
	}
	else if (myops == 4)
	{
		pr_alert("Division: %d\n",myint1/myint2);
	}
	/*for (;i<count;i++)
	{
		pr_info("\tmyarr[%d]= %ld \n", i, myarr[i]); //to print the input values
	}*/

	return 0;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye from mod_arit_operation!\n");
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module for operations");
