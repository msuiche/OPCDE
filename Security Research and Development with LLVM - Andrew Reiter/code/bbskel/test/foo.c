#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void
leaks_passwd()
{
	char *p;
	struct addrinfo hints, *result;

	p = getpass("enter passwd: ");
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	(void)getaddrinfo(p, "http", &hints, &result);
}

int
main(int argc, char **argv)
{
	leaks_passwd();
	return 0;
}
