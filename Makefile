obj-m += sys_xvector.o

INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: clone sys_xvector tester tester_unlink

clone: clone.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi clone.c -o clone

tester: tester.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi tester.c -o tester

tester_unlink: tester_unlink.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi tester_unlink.c -o tester_unlink

sys_xvector:
	make -Wall -Werror -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f clone tester tester_unlink
