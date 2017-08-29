/*
 * NSDL002
 *
 * Naive Sensitive Data Leak 002
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
	char *p;
	struct addrinfo hints, *result;

	p = getpass("enter passwd: ");
	/* l.v. p is now tainted with sensitive data */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;
	/* leak password via getaddrinfo() DNS lookup. contrived af. */
	memset(p, 0, strlen(p)); // XXX :PpPp
	(void)getaddrinfo(p, "http", &hints, &result);
}
int
main(int argc, char **argv)
{
	leaks_passwd(random());
	return 0;
}
