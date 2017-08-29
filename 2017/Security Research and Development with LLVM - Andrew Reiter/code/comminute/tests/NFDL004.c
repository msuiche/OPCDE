/*
 * NFDL004
 *
 * Naive File Descriptor Leak 004
 *
 * One leaks one does not
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
	int fd, fd2;


	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		perror("socket");
	}
	fd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (fd2 == -1) {
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
