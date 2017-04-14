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
	int ln = -1;
	char file_link[1024], file_linkto[1024], file_unlink[1024], ans_link, ans_unlink;

	printf("Would you like to link? [y/N] : ");
	scanf(" %c", &ans_link);

	if (ans_link == 'y' || ans_link == 'Y') {
		printf("File path to link : ");
		scanf("%s", file_link);

		printf("File path to link to : ");
		scanf("%s", file_linkto);

		ln = link(file_link, file_linkto);
		if (ln < 0)
			printf("Unable to link file : %d\n", errno);
	}

	printf("Would you like to unlink? [y/N] : ");
	scanf(" %c", &ans_unlink);

	if (ans_unlink == 'y' || ans_unlink == 'Y') {
		printf("File path to unlink : ");
		scanf("%s", file_unlink);

		ln = unlink(file_unlink);
		if (ln < 0)
			printf("Unable to unlink file : %d\n", errno);
	}

	return 0;
}
