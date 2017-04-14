#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd[3] = {-1, -1, -1}, ret_val = 0, i = 0, size = -1;
	char files[3][1024], ans = 'y';
	char buf_write[20] = "Oh look ..";
	char buf_read[20];

	do {
		for (i = 0; i < 3; ++i) {
			printf("Enter file path for %d : ", i + 1);
			scanf("%s", files[i]);
			fd[i] = open(files[i], O_RDWR | O_APPEND);
			if (fd[i] < 0) {
				printf("Unable to open file %d : %d\n", i + 1, errno);
				ret_val = -errno;
			}
		}

		for (i = 0; i < 3; ++i) {
			size = write(fd[i], buf_write, 10);
			if (size < 0)
				printf("Unable to write file %d : %d\n", i + 1, errno);

			lseek(fd[i], 0, SEEK_SET);
			size = read(fd[i], buf_read, 10);
			if (size < 0)
				printf("Unable to read file %d : %d\n", i + 1, errno);
		}

		for (i = 0; i < 3; ++i) {
			if (close(fd[i]) != 0)
				printf("Unable to close file %d : %d\n", i + 1, errno);
		}

		printf("Would you like to continue? [y/N] : ");
		scanf(" %c", &ans);
	} while (ans == 'y' || ans == 'Y');

	return ret_val;
}
