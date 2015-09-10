#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#include <asm/io.h>
#include <linux/ioport.h>

#include "bcm2835.h"

//Blinks on RPi Plug P1 pin 11
#define PIN RPI_GPIO_P1_11

int openstate = 0;

static int leds_open(struct inode *inode,struct file *filp)
{
    if(openstate == 0)
    {
        openstate = 1;
	printk("open file success!\n");
	return 0;
   }
    else
    {
	printk("file has opened!\n");
	return -1;
    }
}

static int leds_ioctl(struct file *filp,unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
	case 0:
	    bcm2835_gpio_clr(PIN);
	    printk("LED OFF\n");
	    break;
	case 1:
	    bcm2835_gpio_set(PIN);
	    printk("LED ON\n");
	    break;
	default:
	    return -EINVAL;
    }
    return 0;
}

static int leds_release(struct inode *inode,struct file *filp)
{
    if(openstate == 1)
    {
	openstate = 0;
	printk("file close success\n");
	return 0;
    }
    else
    {
	printk("the file has closed\n");
	return -1;
    }
}

static const struct file_operations leds_fops = {
    .owner = THIS_MODULE,
    .open = leds_open,
    .unlocked_ioctl = leds_ioctl,
    .release = leds_release,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "mleds",
    .fops = &led_fops,
};

static int __init leds_init(void)
{
    int ret;

    ret = misc_register(&misc);

    bcm2835_gpio_fsel(PIN,BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_set(PIN);

    printk("leds init.\n");

    return ret;
}

static void leds_exit(void)
{
    bcm2835_gpio_clr(PIN);

    misc_deregister(&misc);

    printk("leds exit\n");
}

module_init(leds_init);
module_exit(leds_exit);

MODULE_AUTHOR("HU");
MODULE_LICENSE("GPL");


















