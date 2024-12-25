ifneq ($(KERNELRELEASE),)

obj-m   := hello.o
ccflags-y += -g
else

KDIR ?= /home/viktor/repos/linux-stable

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello.ko hello.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$$PWD $@

endif