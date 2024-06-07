/* Kernel thread basics */
/*In this program we are printing the pr_info in the "thread_func" after every 1 second*/


#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define TIME_INTVL	(1)	// milliseconds

// define thread structs
static struct task_struct *thread1, *thread2;
static char thread1_name[] = "Thread-1";
static char thread2_name[] = "Thread-2";

int thread_func(void *pv)
{
	int i = 0;
	while(!kthread_should_stop())
	{
		msleep(TIME_INTVL);
		if(i==1000)
		{
			pr_info("In thread %s, i=%d\n", (char*)pv, i++);
		}
		else
		{
			++i;
		}	
	}
	pr_info("Exiting Thread\n");
//	do_exit(0);
	return 0;
}

static int __init my_mod_init(void)
{
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);

	thread1 = kthread_run(thread_func, thread1_name, thread1_name);
	if (thread1)
		pr_info("Thread %s created!\n", thread1_name);
	else
	{
		pr_err("Cannot create thread %s\n", thread1_name);
		goto r_thread1;
	}

	thread2 = kthread_run(thread_func, thread2_name, thread2_name);
	if (thread2)
		pr_info("Thread %s created!\n", thread2_name);
	else
	{
		pr_err("Cannot create thread %s\n", thread2_name);
		goto r_thread2;
	}
	return 0;

r_thread2:
	kthread_stop(thread1);
r_thread1:
	return -1;
}

static void __exit my_mod_exit(void)
{
	kthread_stop(thread1);
	kthread_stop(thread2);
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel threads!");

