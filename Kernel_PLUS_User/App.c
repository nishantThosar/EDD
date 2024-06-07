/* User space file to test userspace / kernelspace data exchange module */

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#define DEV	"/dev/cdac_dev"


int main()
{
	Person person;
	int fd = open(DEV, O_RDWR);
	if (fd<0)
	{
		perror("Cannot open device file");
		exit(EXIT_FAILURE);
	}
	
	printf("Enter Person Name : ");
	__fpurge(stdin);
	fgets(person.name, 20, stdin);

	printf("Address : ");
	__fpurge(stdin);
	fgets(person.Address, 100, stdin);

	printf("Phone No : ");
	__fpurge(stdin);
	fgets(person.name, 10, stdin);
	

	printf("Writing to driver\n");
	ssize_t nwrite = write(fd, &person, sizeof(person));
	
	memset(&person, 0, sizeof(person));

	int nread = read(fd, &person, sizeof(person));
	printf("\n");

	printf("Name : %sAddress : %sPhone : %s", person.name,person.Address, person.phone);

	close(fd);


	return (EXIT_SUCCESS);
}


