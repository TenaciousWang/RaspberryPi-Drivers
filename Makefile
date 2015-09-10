ifneq ($(KERNELRELEASE),)

MODULE_NAME := mled
RESMAN_CORE_OBJS:= led.o
RESMAN_GLUE_OBJS:= bcm2835.o 
$(MODULE_NAME)-objs:=$(RESMAN_GLUE_OBJS) $(RESMAN_CORE_OBJS)
obj-m := mled.o

else

KDIR := /home/wxh/Projects/LinuxKernel/linux-rpi-3.12
all:
	make -C $(KDIR) M=$(PWD) -B /usr/arm-linux-gnueabi/include modules ARCH=arm CROSS_COMPILE=/home/wxh/Projects/LinuxKernel/tools-master/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- 

clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*
endif
