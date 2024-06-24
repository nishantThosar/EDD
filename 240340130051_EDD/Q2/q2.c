/* Kernel linked list
	As conformed with course co-ordinator no user space program will be made for this program.
	In this device driver program we are creating 10 nodes (10 is the limit by default).
	If the array elements(data for linked list) > limit(ll_lim) then module will be not insmodded.
	The limit of linked list can be changed by changing the value of "ll_lim" at the time of insmod.
*/ 

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

#define SIZE			(20)//size of array

static int ll_lim 		= 10;//Linklist limit = 10
static int count 		= 0; // for keeping the count 
static long myarr[SIZE] = {0}; //array
static int j 			= 0; //for loop var
module_param_array(myarr, long, &count, S_IRUGO | S_IWUSR); // for user input at insmod of linked list data
module_param(ll_lim, int, S_IRUGO | S_IWUSR); // for increasing the limit of linklist
MODULE_PARM_DESC(myarr, "Array for inserting data for linked list");
MODULE_PARM_DESC(ll_lim, "Int var for incrimenitng the size of linked list (default is 10)");

// linked list node definition
struct my_node
{
	int data;
	struct list_head my_list;
};

// create list head pointer/node
LIST_HEAD(my_head);
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++INIT++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static int __init my_mod_init(void)
{
	int data;
	struct my_node *node, *tmp;
	pr_info("For myarr, we got %d entries\n", count);
	if (count == 0 || count > 20) // checking if the user has inserted any data for array
	{
		pr_err("Insert data for node in array named myarr.\n");
		return -1;
	}
	if (count == ll_lim || count > ll_lim)  // checking if the user has excedded the linked list limit
	{
		pr_alert("Linked list limit is %d cannot add node\n", ll_lim);
		return -1;
	}
	if (ll_lim == 10) // if user hasen't incrimented the size of linked list
	{
		pr_info("No change for limit found \n");
	}
	pr_info("Limit set to %d\n", ll_lim);
	pr_info("Hello world from %s!\n", KBUILD_MODNAME);
	data = myarr[0];
	/*In this loop nodes will be added and stored dynamically*/
	for (;j<count;j++)
	{
		// kmalloc node
		node = (struct my_node *)kzalloc(sizeof(struct my_node), GFP_KERNEL);
		if(node == NULL) // null check
		{
			pr_alert("Memory allocation failed\n");
			goto k_free;
		}
		node->data = data;
		// enable its 'linked list state'
		INIT_LIST_HEAD(&node->my_list);
		// add node at tail
		list_add(&node->my_list, &my_head);
		data = myarr[j+1];	
	}//for ends
	
	// forward traversal
	pr_info("Linked list forwards:\n");
	count  = 1;
	list_for_each_entry(tmp, &my_head, my_list)
	{
		pr_info("Node %d, data=%d\n", count++, tmp->data);
	}

	return 0;
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++Error Ladder+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	k_free:
		pr_err("Error occured while entering data\n");
		pr_err("Clearing linked_list\n");
		list_for_each_entry_safe(node, tmp, &entry_List, my_entry_list)
		{
			list_del(&node->my_entry_list);
			kfree(node);
		}
		count=0;
		
		return -1;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++EXIT+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static void __exit my_mod_exit(void)
{
	struct my_node *node, *tmp;

	list_for_each_entry_safe(node, tmp, &my_head, my_list)
	{	// delete the node and free its instantiations
		pr_info("Deleting node with data=%d\n", node->data);
		list_del(&node->my_list);
		kfree(node);
	}
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nishant Thosar");
MODULE_DESCRIPTION("Q2 Linked list");
