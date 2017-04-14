#ifndef IOCTL_CALL_H
#define IOCTL_CALL_H

#include <linux/ioctl.h>
#define DEVICE_NUM 100
#define DEVICE_NAME "ioctl_module"

#define IOCTL_SET_VECTOR _IOR(DEVICE_NUM, 1, int)
#define IOCTL_REMOVE _IOW(DEVICE_NUM, 2, int)
#define IOCTL_LIST_ALL _IOR(DEVICE_NUM, 3, int)
#define IOCTL_LIST_PROCESS _IOR(DEVICE_NUM, 4, int)

struct ioctl_param_s {
	pid_t pid;
	int vec_num;
};

#endif
