obj-m := hello2.o
ccflags-y := -I$(PWD)/../hello1/inc

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$$PWD \
		KBUILD_EXTRA_SYMBOLS=$(PWD)/../hello1/Module.symvers

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
