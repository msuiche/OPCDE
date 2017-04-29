/*
 * NFDL005
 *
 * Naive File Descriptor Leak 005
 *
 * opens and closes a global var fd
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

int fd;

void
leaks_fd()
{
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
