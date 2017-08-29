/*
 * NFDL002
 *
 * Naive File Descriptor Leak 002
 * .. well this does not leak.
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

void
foo(char *k)
{
	return;
}

void
leaks_fd()
{
	int fd;


	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		perror("socket");
	}
	close(fd);
	return;
}

int
main(int argc, char **argv)
{
	leaks_fd();
	return 0;
}
