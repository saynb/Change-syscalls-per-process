#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/sys_xvector.h>
#include "ioctl_call.h"

MODULE_LICENSE("GPL");

extern struct vector_info *vector_info_1_ptr;
extern struct vector_info *vector_info_2_ptr;
extern struct task_struct *find_task_by_vpid(pid_t nr);

static long device_ioctl(struct file *file, unsigned int ioctl_num,
	unsigned long ioctl_param)
{
	int ret = 0;
	struct task_struct *tsk = NULL;
	struct ioctl_param_s *ioctl_param_p = (struct ioctl_param_s *)ioctl_param;
	struct vector_info *vector_info;

	printk("In %s:%d ioctl_param:%lu\n", __func__, __LINE__, ioctl_param);

	switch (ioctl_num) {
	case IOCTL_SET_VECTOR:
		tsk = find_task_by_vpid(ioctl_param_p->pid);
		if (!tsk)
			break;

		printk("Adding vector\n");
		switch (ioctl_param_p->vec_num) {
		case 1:
			if (vector_info_1_ptr) {
				mutex_lock(&vector_info_1_ptr->vector_mutex);
				tsk->vector_syscall = vector_info_1_ptr;
				vector_info_1_ptr->vector_user_count++;
				mutex_unlock(&vector_info_1_ptr->vector_mutex);
			}
			break;
		case 2:
			if (vector_info_2_ptr) {
				mutex_lock(&vector_info_2_ptr->vector_mutex);
				tsk->vector_syscall = vector_info_2_ptr;
				vector_info_2_ptr->vector_user_count++;
				mutex_unlock(&vector_info_2_ptr->vector_mutex);
			}
			break;
		default:
			tsk->vector_syscall = NULL;
			break;
		}
		printk(" Vector id received is: %d\n", ioctl_param_p->vec_num);
		printk(" pid for the process: %d\n", tsk->pid);
		break;
	case IOCTL_REMOVE:
		tsk = find_task_by_vpid(ioctl_param_p->pid);

		if (!tsk)
			break;
		printk("Removing vector\n");
		if (tsk->vector_syscall) {
			vector_info = (struct vector_info *)tsk->vector_syscall;
			mutex_lock(&vector_info->vector_mutex);
			tsk->vector_syscall = NULL;
			vector_info->vector_user_count++;
			mutex_unlock(&vector_info->vector_mutex);
		}

		printk(" Vector id received is: %d\n", ioctl_param_p->vec_num);
		printk(" pid for the process: %d\n", tsk->pid);
		break;
	case IOCTL_LIST_ALL:
		printk("List all vectors\n");
		printk("Vector 1 -> Vector ID:%d User_count = %d\n\n",
		       vector_info_1_ptr->vector_id, vector_info_1_ptr->vector_user_count);
		printk("Vector 2 -> Vector ID:%d User_count = %d\n\n",
		       vector_info_2_ptr->vector_id, vector_info_2_ptr->vector_user_count);
		break;
	case IOCTL_LIST_PROCESS:
		tsk = find_task_by_vpid(ioctl_param_p->pid);
		if (!tsk)
			break;
		printk("List process specific to %d\n", ioctl_param_p->pid);

		printk("For PID %d\n", tsk->pid);

		if (tsk->vector_syscall) {
			vector_info = (struct vector_info *)tsk->vector_syscall;
			printk("Vector ->\n Vector ID:%d\nUser_count = %d\n\n",
			       vector_info->vector_id,
			       vector_info->vector_user_count);
		}
		break;

	default:
		ret = -1;
		break;
	}
	return (long)ret;
}

struct file_operations fops = {
	.unlocked_ioctl = device_ioctl,
};

static int __init init_ioctl_module(void)
{
	int ret = 0;

	ret = register_chrdev(DEVICE_NUM, DEVICE_NAME, &fops);
	printk("%s:%d\n", __func__, __LINE__);
	if (ret < 0) {
		printk("%s failed with %d\n",
		       "Sorry, registering the character device ", ret);
	}
	return ret;
}

/*
 * On exiting, the character device is unregistered.
 */
static void __exit exit_ioctl_module(void)
{
	unregister_chrdev(DEVICE_NUM, DEVICE_NAME);
}

module_init(init_ioctl_module);
module_exit(exit_ioctl_module);
