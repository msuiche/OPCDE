/*
 * NSDL003
 *
 * Naive Sensitive Data Leak 003
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void
leaks_passwd(unsigned lookup)
{
	char *p, *a2l = "www.cw-complex.com";
	struct addrinfo hints, *result;
	if (lookup) {
		p = getpass("enter passwd: ");
	} else {
		p = a2l;
	}
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	(void)getaddrinfo(p, "http", &hints, &result);	/* which path did p take? */
}
int
main(int argc, char **argv)
{
	leaks_passwd(random());
	return 0;
}
