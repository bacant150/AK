// SPDX-License-Identifier: Dual BSD/GPL

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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <asm-generic/bug.h>
#include "hello.h"

MODULE_AUTHOR("Huranets Oleksandr <bacant150@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training with Dynamic Debug");
MODULE_LICENSE("Dual BSD/GPL");

#define BUG_ON_MSG(condition, msg) \
	do { \
		if (unlikely(condition)) { \
			pr_warn(msg); \
			BUG(); \
		} \
	} while (0)

static unsigned int param = 1;
static unsigned int allocations;

struct event {
	struct list_head list;
	ktime_t event_time;
};

module_param(param, uint, 0644);
MODULE_PARM_DESC(param, "An unsigned integer, specifies how many times to write \"Hello, world!\"");
static struct list_head event_list_head;
static LIST_HEAD(event_list_head);

static int __init hello_init(void)
{
	int i = 0;

	if (param == 0 || (param >= 5 && param <= 10))
		pr_warn(" param is 0 [5,10]");

	BUG_ON(param > 10);

	for (i = 0; i < param; i++) {
		pr_emerg("Hello, World!\n");
		new_event();
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct event *md, *tmp;

	list_for_each_entry_safe(md, tmp, &event_list_head, list) {
		pr_emerg("Time: %lld\n", md->event_time);
		list_del(&md->list);
		kfree(md);
	}
}

void new_event(void)
{
	struct event *element;

	element = kmalloc(sizeof(struct event), GFP_KERNEL);
	allocations++;
	BUG_ON_MSG(allocations == 5, "kmallo returned NULL");
	element->event_time = ktime_get();
	list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);