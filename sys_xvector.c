#include <linux/linkage.h>
#include <linux/moduleloader.h>
#include <linux/sched.h>
#include <linux/ptrace.h>
#include <linux/sys_xvector.h>

static int xvector(void);

struct vector_syscall_s vector_1 = {
	.sys_open	 = do_sys_open_1,
	.sys_read	 = read_1,
	.sys_write	 = write_1,
	.sys_close	 = close_1,
	.sys_link	 = link_1,
	.sys_unlink	 = unlink_1,
};

struct vector_syscall_s vector_2 = {
	.sys_open	 = do_sys_open_2,
	.sys_read	 = read_2,
	.sys_write	 = write_2,
	.sys_close	 = close_2,
	.sys_link	 = link_2,
	.sys_unlink	 = unlink_2,
};
extern int clone_global_print_flag;
asmlinkage extern long (*sysptr)(unsigned long clone_flags,
		unsigned long newsp,
		int __user *parent_tidptr,
		int __user *child_tidptr,
		unsigned long tls, int vector_num);

struct vector_info vector_info_1;
struct vector_info vector_info_2;

extern struct vector_info *vector_info_1_ptr;
extern struct vector_info *vector_info_2_ptr;

static int xvector(void)
{
	/*
	 * Allocate memory for vector_info
	 * Register the vectors
	 */

	vector_info_1.vector_id = 0;
	vector_info_1.vector_user_count = 0;
	vector_info_1.vector_syscall = &vector_1;
	mutex_init(&vector_info_1.vector_mutex);

	vector_info_2.vector_id = 1;
	vector_info_2.vector_user_count = 0;
	vector_info_2.vector_syscall = &vector_2;
	mutex_init(&vector_info_1.vector_mutex);

	/*
	 * Keep the vectors in global memory for now
	 */
	vector_info_1_ptr = &vector_info_1;
	vector_info_2_ptr = &vector_info_2;
	return 0;
}

long sys_clone2(unsigned long clone_flags, unsigned long newsp,
		int __user *parent_tidptr,
		int __user *child_tidptr,
		unsigned long tls, int vector_num)
{
	printk("In our syscall %s:%d\n", __func__,  __LINE__);
	printk("clone_flags:  %lu\n", clone_flags);
	printk("parent_tid:  %p\n", parent_tidptr);
	printk("vector_num:  %d\n", vector_num);
	return _do_fork(clone_flags, newsp, 0, parent_tidptr,
			child_tidptr, tls, vector_num);
}

static int __init init_sys_xvector(void)
{
	int status;

	printk("installed new sys_xvector module\n");

	clone_global_print_flag = 0;
	status = xvector();
	if (sysptr == NULL)
		sysptr = sys_clone2;

	return status;
}
static void  __exit exit_sys_xvector(void)
{
	clone_global_print_flag = 0;
	if (sysptr != NULL)
		sysptr = NULL;
	printk("removed sys_xvector module\n");
}
module_init(init_sys_xvector);
module_exit(exit_sys_xvector);
MODULE_LICENSE("GPL");
