/*Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "ioctl_call.h"

struct ioctl_param_s ioctl_param_global;

/* Function which calls octl to deregister vector */
int ioctl_reset_vector(int fd, struct ioctl_param_s *ioctl_param_p)
{
	int ret_val = 0;

	ret_val = ioctl(fd, IOCTL_REMOVE, (void *)ioctl_param_p);
	if (ret_val < 0)
		printf("IOCTL remove factor failed %d %d\n", errno, fd);

	return ret_val;
}

/* Function  which calls ioctl to set vector */
static int ioctl_set_vector(int fd, struct ioctl_param_s *ioctl_param_p)
{
	int ret_val = 0;

	ret_val = ioctl(fd, IOCTL_SET_VECTOR, (void *)ioctl_param_p);
	if (ret_val < 0)
		printf("IOCTL set factor failed %d %d\n", errno, fd);

	return ret_val;
}

/* Function which calls ioctl to list all vectors */
static int ioctl_list_all(int fd, struct ioctl_param_s *ioctl_param_p)
{
	int ret_val = 0;

	ret_val = ioctl(fd, IOCTL_LIST_ALL, (void *)ioctl_param_p);
	if (ret_val < 0)
		printf("Could not list all the vectors %d %d\n", errno, fd);

	return ret_val;

}

/* Functin which calls ioctl to list all process */
static int ioctl_list_process(int fd, struct ioctl_param_s *ioctl_param_p)
{
	int ret_val = 0;

	ret_val = ioctl(fd, IOCTL_LIST_PROCESS, (void *)ioctl_param_p);
	if (ret_val < 0)
		printf("Process specific call failed %d %d\n", errno, fd);

	return ret_val;
}

/* Main function which calls the above functions to register/deregister */
int main(int argc, char *argv[])
{
	int ret_val = 0, fd = -1, opt;
	char proc[] = "/dev/ioctl_module";
	struct ioctl_param_s *ioctl_param_p;

	ioctl_param_p = &ioctl_param_global;

	if (optind > 2) {
		printf("More than one flags passed. Exiting ...\n");
		goto err_out;
	}
	if (argv[2])
		ioctl_param_p->pid = atoi(argv[2]);
	if (argv[3])
		ioctl_param_p->vec_num = atoi(argv[3]);

	fd = open(proc, O_CREAT | O_RDWR, 777);
	if (fd < 0) {
		printf("File %s cannot be opened\n", proc);
		ret_val = -EINVAL;
		goto err_out;
	}
	/*
	 * argument parsing logic for options
	 */
	while ((opt = getopt(argc, argv, "ahprs")) != -1) {
		switch (opt) {
		case 'a':
			ret_val = ioctl_list_all(fd, ioctl_param_p);
			goto err_out;
		case 'h':
			printf("Sample command :\n");
			printf("./ioctl_call -[ahlpr] pid vec_num\n\n");
			printf("Possible flag values -\n\n");
			printf("a: list all vectors\n");
			printf("h: print help for ioctl_call\n");
			printf("p: list vector for a particular process\n");
			printf("r: reset a vector for a process\n");
			printf("s: set a vector for a process\n");
			goto err_out;
		case 'p':
			ret_val = ioctl_list_process(fd, ioctl_param_p);
			goto err_out;
		case 'r':
			ret_val = ioctl_reset_vector(fd, ioctl_param_p);
			goto err_out;
		case 's':
			ret_val = ioctl_set_vector(fd, ioctl_param_p);
			goto err_out;
		default:
			printf("Unknown option encountered! Exiting ...\n");
			goto err_out;
		}
	}

err_out:
	return ret_val;
}
