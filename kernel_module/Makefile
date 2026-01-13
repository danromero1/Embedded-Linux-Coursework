obj-m += test_mod01.o

KDIR=/usr/src/linux-headers-6.14.0-1016-raspi

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
	rm -f *.o
