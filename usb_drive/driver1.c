/*USB driver vsrrsion 1*/
#include <linux/module.h>
#include <linux/inti.h>
#include <linux/usb.h>

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt //just to print the module name in dmesg

/*Driver Info*/
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Nishant Thosar");
MODULE_DESCRIPTION("Driver for usb stick");

#define VEND_ID 0x03f0 
#define PROD_ID 0x2003

static struct usb_device_id my_table[] = {
	{USB_DEVICE(VEND_ID, PROD_ID)},
	// {USB_DEVICE_INFO(class, subclass, protocoll)},
	{} 
};

MODULE_DEVICE_TABLE(usb, my_table);

static int my_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	pr_alert("Probe function called\n");
	return 0;
}
static int my_disconnect(struct usb_interface *intf)
{
	pr_alert("Discoonnect function called\n");
	return 0;
}

stati struct usb_driver my_driver = {
	.name       = "My_usb_driver",
	.id_table   = my_table,
	.probe      = my_probe,
	.disconnect	= my_disconnect,
};

static int __init main(void)
{
	int result;
	pr_info("Driver called\n");
	result = usb_register(&my_usb_driver);
	if (result)
	{
		pr_info("Error in driver init");
		return -result;
	}
	return 0;
}

static void __exit remove(void)
{
	pr_info("Driver removed\n");
	usb_deregister(&my_usb_driver);
}

module_init(main);
module_exit(remove);
