#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt // tp print the module name in dmesg

#include <linux/module.h>
#include <linux/moduleparam.h> // for parameter support
#include <linux/init.h>

#define SIZE	(10) // so that not more than 10 inputs are taken by user if passed >10 then only 10 values will be taken by kernal
static long myarr[SIZE] = {0};
static int count = 0; // will contain how many array entries were received
static int ops = 10; 

module_param(ops, int, S_IRUGO | S_IWUSR);
module_param_array(myarr, long, &count, S_IRUGO | S_IWUSR);

MODULE_PARM_DESC(ops, "This is an integer array");//description of array

static int __init my_mod_init(void)
{
	int  i=0; int j =0; int k = 0;
	pr_info("Function %s called\n", __func__);//just to print which function is called.

	if (ops == 10) // to print the array once the module is inserted
	{
		for (; i < count; ++i)
		{
			pr_info("\nArr[%d]=%ld\n",i, myarr[i]);
		}//for ends
	}//if ends

	else if (ops == 1)//to print the 
	{
		int max = 0;

		for (; j < count; ++j)
		{
			pr_info("\nArr[%d]:%ld\n", j, myarr[j]);
			max = myarr[0];
			if (max < myarr[j])
			{
				max = myarr[j];
			}//if ends
		}//for ends
		pr_alert("\nMAX value = %d\n", max);
	}//if ends

	else if (ops == 2)//to print the 
	{
		int max = 0;

		for (; k < count; ++k)
		{
			pr_info("\nArr [%d]: %ld\n", k, myarr[k]);
			max = myarr[0];
			if (max > myarr[k])
			{
				max = myarr[k];
			}//if ends
		}//for ends
		pr_alert("\nMIN value = %d\n", max);
	}//if ends
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
