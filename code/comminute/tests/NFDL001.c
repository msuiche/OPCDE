/*
 * NFDL001
 *
 * Naive File Descriptor Leak 001
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

void
leaks_fd()
{
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		perror("socket");
	}
	return;
}

int
main(int argc, char **argv)
{
	leaks_fd();
	return 0;
}
