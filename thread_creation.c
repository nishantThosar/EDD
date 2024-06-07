/* Kernel thread basics */
/*In this program we are creating 6 threads ans all the threads are using the same function so
  the even threads are getting the delay (number) which is assigned to eve_sleep var and the 
  odd threads are getting the delay which value is assigned to odd_sleep.
  Default case: if no inputs are given during the insmod then the threads will sleep at 1 seconds by default

Name : Ashish Bansal
Place CDAC
Date: 7 June 2024 (Friday)

*/



#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt //// just to print the kernel module name

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define MAX_THREADS	(6) //for max number of threads

//struct for sleep
struct thread_info_driver
{
	int num;
	int sleep;
};

struct thread_info_driver info[6] = {0}; // array for struct

static int eve_sleep = 1;//odd sleep var
static int odd_sleep = 1;//even sleep var

// define thread structs
static struct task_struct *thread[MAX_THREADS]; //struct array
static char thread_name[20]; //thread name buffer

//to enter the var during the time of insmod
module_param(eve_sleep, int, S_IRUSR | S_IWUSR);
module_param(odd_sleep, int, S_IRUSR | S_IWUSR);

//description of both var
MODULE_PARM_DESC(eve_sleep, "Sleep (in sec) for even threads");
MODULE_PARM_DESC(odd_sleep, "Sleep (in sec) for odd threads");

//Thread function
/*In this function we are taking a void pointer for detecting the thread (odd or even)*/
int thread_func(void *pv)
{
	struct thread_info_driver info = *(struct thread_info_driver*)pv;
	info.sleep *= 1000; // to multiply the input (eve_sleep || odd_sleep) so as to convert it in milliseconds
	
	while(!kthread_should_stop())
	{	
		msleep(info.sleep); //milli function to sleep 
		pr_info("Slept %d %d\n", info.sleep, info.num); // to print how much time the thread slept
	}
	pr_info("Exiting Thread %d\n", info.num);
	return 0;
}

static int __init my_mod_init(void)
{
	int  i=0;
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);

	// to create 6 threads
	for(i = 0; i<MAX_THREADS; ++i)
	{
		sprintf(thread_name, "%s%d", "Thread", i+1); 
		info[i].num = i; //to get the even or odd of therad
		info[i].sleep = i%2 ? odd_sleep: eve_sleep;//if the thread is odd then odd sleep will be assigned & vice versa
		thread[i] = kthread_run(thread_func, (void*)&info[i], thread_name); // to create and run the threads
		if (thread[i])
			pr_info("Thread %s created!\n", thread_name); // to print number of threads created
		else
		{
			pr_err("Cannot create thread %s\n", thread_name);
			goto r_thread;
		}
	}
	return 0;
//error ladder
r_thread:
	for(i = i-1; i>=0; --i)
	{
		kthread_stop(thread[i]);
	}
	return -1;
}

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

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel threads!");

