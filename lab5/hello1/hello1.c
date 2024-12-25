// SPDX-License-Identifier: Dual BSD/GPL

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "hello1.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Huranets Oleksandr <bacant150@gmail.com>");
MODULE_DESCRIPTION("Hello1 module: prints hello and measures time");

struct hello_data {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(hello_list);

int print_hello(uint param) 
{
	struct hello_data *item;
	uint i;

	if (param == 0) {
		pr_warn("Warning: param is 0\n");
	} else if (param >= 5 && param <= 10) {
		pr_warn("Warning: param is between 5 and 10\n");
	} else if (param > 10) {
		pr_err("Error: param is greater than 10\n");
		return -EINVAL;
	}

	for (i = 0; i < param; i++) {
		item = kmalloc(sizeof(*item), GFP_KERNEL);
		if (!item)
			return -EINVAL;

		item->time_before = ktime_get();
		pr_info("Hello, world!\n");
		item->time_after = ktime_get();

		list_add_tail(&item->list, &hello_list);
	}

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_data *item, *tmp;

	list_for_each_entry_safe(item, tmp, &hello_list, list) {
		pr_info("Time delta: %lld ns\n",
			ktime_to_ns(
				ktime_sub(
					item->time_after, item->time_before)));
		list_del(&item->list);
		kfree(item);
	}
	pr_info("hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
