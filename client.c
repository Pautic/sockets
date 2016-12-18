#include <sys/socket.h>		/* socket() */
#include <netinet/in.h>		/* struct sockaddr_in & struct in_addr ? */
#include <unistd.h>		/* close() */
#include <netdb.h>		/* struct addrinfo */
#include <stdio.h>		/* perror */
#include <string.h>		/* memset */
#include <math.h>	/* pow() */

#include "sockets.h"

struct sockclt {
	int fd;
	struct addrinfo hints;
	struct addrinfo *info;
};

int main(int argc, char *argv[])
{
	unsigned int pub = 0x4d07;
	unsigned int prv = 0x4d2b;
	unsigned int msg = 5;

	printf("pow %f\n", pow(47, 43));
	printf("ipow %llu\n", ipow(47, 43));

	printf("msg : %u\n", msg);
	msg = crypt(pub, msg);
	printf("msg : %u\n", msg);
	msg = crypt(prv, msg);
	printf("msg : %u\n", msg);

	/* Port, get user input else default port */
	char b = 6;
	char port[b];
	unsigned int portuint = (argc == 2) ? get_port(argv) : defport;
	snprintf(port, b, "%u", portuint); /* uint to char[] for getaddrinfo */

	/* Messages */
	char rmsg[16];
	char smsg[] = "Key: 123";

	/* File descriptor & addresses */
	struct sockclt clt;

	/* See GETADDRINFO(3) */
	memset(&clt.hints, 0, sizeof(clt.hints));
	clt.hints.ai_family 	= addrfam;
	clt.hints.ai_socktype 	= SOCK_STREAM;
	clt.hints.ai_protocol 	= 0;
	clt.hints.ai_flags 	= AI_NUMERICSERV;

	/* Node = NULL -> INADDR_LOOPBACK */
	if (getaddrinfo(NULL, port, &clt.hints, &clt.info) != 0) {
		perror("Getaddrinfo");
		return 0;
	}

	clt.fd = socket(clt.info->ai_family,
			clt.info->ai_socktype,
			clt.info->ai_protocol);

	if (clt.fd == -1) {
		perror("Socket");
	} else {
		if (connect(	clt.fd,
				clt.info->ai_addr,
				clt.info->ai_addrlen) == -1) {
			perror("Connect");
		} else {
			if (send(clt.fd, &smsg, sizeof(smsg), 0) == -1)
				perror("Send");

			if (recv(clt.fd, &rmsg, sizeof(rmsg), 0) == -1)
				perror("Recv");

			printf("%s\n", rmsg);
		}

		if (close(clt.fd) != 0)
			perror("Close");
	}

	freeaddrinfo(clt.info);

	return 0;
}
