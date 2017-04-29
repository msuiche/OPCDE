/*
 * NFDL005
 *
 * Naive File Descriptor Leak 005
 *
 * global variable that ``seems'' to leak.
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
	return;
}

int
main(int argc, char **argv)
{
	leaks_fd();
	return 0;
}
