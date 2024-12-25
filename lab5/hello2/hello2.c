// SPDX-License-Identifier: Dual BSD/GPL

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Huranets Oleksandr <bacant150@gmail.com>");
MODULE_DESCRIPTION("Hello2 module: uses hello1 module's print_hello function");

static uint count = 1;

module_param(count, uint, 0644);
MODULE_PARM_DESC(count, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
	pr_info("hello2 module loaded\n");
	print_hello(count);
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
