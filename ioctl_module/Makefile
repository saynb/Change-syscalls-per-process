obj-m += ioctl_module.o

INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: ioctl_call ioctl_module

ioctl_call: ioctl_call.c
	gcc -g -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi ioctl_call.c -o ioctl_call

ioctl_module: ioctl_module.c
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ioctl_call
