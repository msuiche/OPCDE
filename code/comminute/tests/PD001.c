/*
 * PD001
 *
 * Potentially Dangerous 001
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void
blind_copy()
{
	char buf[512];
	char buf2[32];
	struct addrinfo hints, *result;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	(void)getaddrinfo("www.evilgoogle.com", "http", &hints, &result);
	int sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sfd == -1) {
		return;
	}
	memset(&buf, 0, 512);	
	connect(sfd, result->ai_addr, result->ai_addrlen);
	read(sfd, &buf, 511);
	strcpy(buf2, buf); 	
	
}

int
main(int argc, char **argv)
{
	blind_copy();
	return 0;
}
