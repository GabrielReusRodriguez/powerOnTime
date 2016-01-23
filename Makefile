#MakeFile

KERNEL_VERSION = $(shell uname -r)
obj-m += powerOnTime.o

all:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean

module_install:
	cp ./powerOnTime.ko /lib/modules/$(KERNEL_VERSION)/extra/powerOnTime.ko
