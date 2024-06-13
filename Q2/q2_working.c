/*EDD Assgmt #2: LKM - Kernel threads

Write an LKM that creates / spawns kernel threads. 

The module should accept the following module parameters:
1. The number of kernel threads will be taken as an input (unsigned integer): num_threads
2. Period of odd numbered threads period_odd
3. Period of even numbered threads: period_even

Number the kernel threads as odd (1,3,5,...) or even (0,2,4,...). Each thread should print its name and an ever increasing per-thread count (count) in dmesg at the respective period (odd / even).

The module should load and unload cleanly without leaving any leaks or zombie threads. Test the module with various input parameters and upload some results.
*/


#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>
// #define MAX_THREADS	(6) //for max number of threads
/*--------------------------------------FUNC PROTOTYPES------------------------------------------------*/
static int __init my_mod_entry(void);
module_init(my_mod_entry);
static void __exit my_mod_exit(void);
module_exit(my_mod_exit);
int thread_func(void *pv);

//-------------------------------------- STRUCT FOR THREAD INFO ------------------------------------------
struct thread_info_driver
{
	int num;
	int sleep;
};
//--------------------------------------- GLOBAL DECLARAIONS ----------------------------------------------
static int eve_sleep = 1;//odd sleep var
static int odd_sleep = 1;//even sleep var
static int MAX_THREADS = 2;

// define thread structs
static struct task_struct *thread[10]; //struct array
static char thread_name[20]; //thread name buffer
struct thread_info_driver info[10] = {0}; // array for struct with name info.
module_param(eve_sleep, int, S_IRUSR | S_IWUSR);
module_param(odd_sleep, int, S_IRUSR | S_IWUSR);
module_param(MAX_THREADS, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(eve_sleep, "Sleep (in sec) for even threads | Range(0 to 9999)");
MODULE_PARM_DESC(odd_sleep, "Sleep (in sec) for odd threads | Range(0 to 9999)");
MODULE_PARM_DESC(MAX_THREADS, "MAX number of threads | Range(0 to 100)");

/*------------------------------------------------ INIT function --------------------------------------------------*/
static int __init my_mod_entry(void)
{
	int  i=0;
	if ((eve_sleep < 0) && (eve_sleep > 10000))
	{
		pr_emerg("Error enter sleep in range 0 to 9999\n");
		return -1;
	}
	if ((MAX_THREADS < 2) || (MAX_THREADS > 100) || (MAX_THREADS == 0))
	{
		pr_notice("Thread number cannot be zero or negative \n Range(3 to 100)\n");
		return -1;
	}
	else
	pr_alert("Now only two threads will be made");
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	//creating threads 
	for(i = 0; i<MAX_THREADS; ++i)
	{
		sprintf(thread_name, "%s%d", "Thread", i+1);	
		info[i].num = i;
		info[i].sleep = (i%2) ? odd_sleep: eve_sleep;//if the thread is odd then odd sleep will be assigned & vice versa
		thread[i] = kthread_run(thread_func, (void*)&info[i], thread_name); // to create and run the threads
		if (thread[i])
			pr_info("Thread %s created!\n", thread_name);
		else
		{
			pr_err("Cannot create thread %s\n", thread_name);
			goto r_thread;
		}
	}
	return 0;
//+++++++++++++++++++++++++++++ error ladder ++++++++++++++++++++++++++++++++++++++++
r_thread:
	for(i = i-1; i>=0; --i)
	{
		kthread_stop(thread[i]);
	}
	return -1;
}

/*---------------------------------------------------- EXIT FUNCTION ------------------------------------------------*/
static void __exit my_mod_exit(void)
{
	int i;
	for(i = 0; i<MAX_THREADS; ++i)
	{
		kthread_stop(thread[i]);
	}
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}
/*---------------------------------------------------THREAD FUNCTION-------------------------------------------------*/
int thread_func(void *pv)
{
	struct thread_info_driver info = *(struct thread_info_driver*)pv;
	info.sleep *= 1000; // to multiply the input (eve_sleep || odd_sleep) so as to convert it in milliseconds
	
	while(!kthread_should_stop())
	{	
		msleep(info.sleep);
		pr_info("Slept %d %d\n", info.sleep, info.num);
	}
	pr_info("Exiting Thread %d\n", info.num);
//	do_exit(0);
	return 0;
}//thread_func ends
/*_______________________________________________________ END _________________________________________________________*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel threads!");
