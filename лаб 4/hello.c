// SPDX-License-Identifier: GPL-2.0

/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Huranets Oleksandr <bacant150@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint param = 1;
module_param(param, uint, 0644);
MODULE_PARM_DESC(param, "An unsigned integer, specifies how many times to write \"Hello, world!\"");

struct my_data {
	struct list_head list;
	ktime_t ktime;
};

static LIST_HEAD(my_list);

static int __init hello_init(void)
{
	uint i;

	if (param == 0 || (param > 5 && param <= 10)) {
		pr_warn("Warning: param is 0 or between 5 and 10!\n");
	} else if (param > 10) {
		pr_err("Error: param is greater than 10! Module not loaded.\n");
		return -EINVAL;
	}

	for (i = 0; i < param; i++) {
		struct my_data *new = kmalloc(sizeof(*new), GFP_KERNEL);

		if (!new)
			return -ENOMEM;

		new->ktime = ktime_get();

		list_add_tail(&new->list, &my_list);

		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct my_data *node, *tmp;

	list_for_each_entry_safe(node, tmp, &my_list, list) {
		pr_info("Time: %lld ns\n", ktime_to_ns(node->ktime));
		list_del(&node->list);
		kfree(node);
	}
}

module_init(hello_init);
module_exit(hello_exit);
