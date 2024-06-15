/* User space app to test IOCTL */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <errno.h>

#include "my_ioctls.h"

#define DEV	"/dev/cdac_dev"
#define BUF_SZ	(10)

int main()
{
	int fd = open(DEV, O_RDWR);
	if (fd<0)
	{
		perror("Cannot open device file");
		exit(EXIT_FAILURE);
	}

	unsigned int to_send = 1;
	int ans = ioctl(fd, START_DEVICE, (unsigned int*)&to_send);
	if (ans<0)
	{
		perror("IOCTL read failure");
		exit(EXIT_FAILURE);
		close(fd);
	}
	printf("Send value: %d to kernel via IOCTL\n", to_send);
	sleep (5); // to sleep the program
	unsigned int stop = 2;
	ans = ioctl(fd, STOP_DEVICE, (unsigned int *)&stop);
	if (ans<0)
	{
		perror("IOCTL read failure");
		exit(EXIT_FAILURE);
		close(fd);
	}
	close(fd);

	printf("Received value %d from kernel via IOCTL\n", received);

	return (EXIT_SUCCESS);
}

