/*
 * NFDL003
 *
 * Naive File Descriptor Leak 003
 *
 * copies fd to another local variable and closes that... so no leak
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
	int fd2;

	fd2 = 0;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		perror("socket");
	}
	fd2 = fd;
	close(fd2);
	return;
}

int
main(int argc, char **argv)
{
	leaks_fd();
	return 0;
}
