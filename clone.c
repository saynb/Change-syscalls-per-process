#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <unistd.h>

#ifndef __NR_clone2
#error xvector system call not defined
#endif

#define FIBER_STACK 2048
#define CLONE_SYSCALLS  0x00001000

void *stack;
int do_something(void)
{
	int a = 0;

	while (a < 10)
		printf("pid : %d, a = %d\n", getpid(), a++);

	exit(1);
}
int main(int argc, const char *argv[])
{
	void *stack;
	int a = 0;
	int rc;

	stack = malloc(FIBER_STACK);
	if (!stack) {
		printf("The stack failed\n");
		exit(0);
	}

	printf("func pointer : %p\n", &do_something);
	if (a == 0) {
		rc = syscall(__NR_clone2, CLONE_SYSCALLS,
			     (char *)stack + FIBER_STACK,
			     1, 1, 1, 1);
	}
	if (rc == 0)
		printf("syscall returned %d\n", rc);
	else
		printf("syscall returned %d (errno=%d)\n", rc, errno);
	while (a < 10)
		printf("main pid : %d, a = %d\n", getpid(), a++);
	return 1;
}
