/*
 *  A keylogger module
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/keyboard.h>     /* Needed for the notifier_block */
#include <linux/kmod.h>

#define DRIVER_AUTHOR "Nick Mullican"
#define DRIVER_DESC   "A Simple Key press counter"
#define DRIVER_LICENSE "GPL"

static int altKeyDepressed = 0;
static int ctrlKeyDepressed = 0;
static int duressButtonPresses = 0;

//Left CTRL 29
//Left ALT  56
//Right ALT 100
//Right CTRL 97
//p 25

int keylogger_notify(struct notifier_block *nblock, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;
    if (code == KBD_KEYCODE)
    {
        if(param->down)
        {
            if(param->value == 29 || param->value == 97)
            {
                printk(KERN_INFO "CTRL Key Down\n");

                ctrlKeyDepressed = 1;
            }
            else if(param->value == 56 || param->value == 100)
            {
                printk(KERN_INFO "ALT Key Down\n");

                altKeyDepressed = 1;
            }
            else if(param->value == 25 && altKeyDepressed == 1 && ctrlKeyDepressed == 1)
            {
                printk(KERN_INFO "P Press with Alt and Ctrl Down\n");

            }
        }
        else if(!param->down)
        {
            if(param->value == 29 || param->value == 97)
            {
                printk(KERN_INFO "CTRL Key Up\n");

                ctrlKeyDepressed = 0;
            }
            else if(param->value == 56 || param->value == 100)
            {
                printk(KERN_INFO "ALT Key Up\n");

                altKeyDepressed = 0;
            }
        }
    }

    return NOTIFY_OK;
}

static struct notifier_block keylogger_nb =
{
    .notifier_call = keylogger_notify
};


static int __init init_keylogger(void)
{
    /* Register this module with the notification list maintained by the keyboard driver.
    This will cause our "keylogger_notify" function to be called upon every key press and release event. This call is non-blocking.
    */
    register_keyboard_notifier(&keylogger_nb);
    printk(KERN_INFO "Registering the keylogger module with the keyboard notifier list\n");
    return 0;
}

static void __exit cleanup_keylogger(void)
{
    unregister_keyboard_notifier(&keylogger_nb);
    printk(KERN_INFO "Unregistered the keylogger module \n");
}

module_init(init_keylogger);
module_exit(cleanup_keylogger);
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("Not machine dependent");

