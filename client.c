#include <sys/socket.h>		/* socket() */
#include <netinet/in.h>		/* struct sockaddr_in & struct in_addr ? */
#include <unistd.h>		/* close() */
#include <netdb.h>		/* struct addrinfo */
#include <stdio.h>		/* perror */
#include <string.h>		/* memset */

#include "sockets.h"

struct sockcli {
	int fd;
	struct addrinfo hints;
	struct addrinfo *info;
};

int main(int argc, char *argv[])
{
	/* Port, get user input else default port */
	char b = 6;
	char port[b];
	unsigned int portuint = (argc == 2) ? get_port(argv) : defport;
	snprintf(port, b, "%u", portuint); /* uint to char[] for getaddrinfo */

	/* Messages */
	char rmsg[16];
	char smsg[] = "Key: 123";

	/* File descriptor & addresses */
	struct sockcli client;

	/* See GETADDRINFO(3) */
	memset(&client.hints, 0, sizeof(client.hints));
	client.hints.ai_family 		= addrfam;
	client.hints.ai_socktype 	= SOCK_STREAM;
	client.hints.ai_protocol 	= 0;
	client.hints.ai_flags 		= AI_NUMERICSERV;

	/* Node = NULL -> INADDR_LOOPBACK */
	if (getaddrinfo(NULL, port, &client.hints, &client.info) != 0) {
		perror("Getaddrinfo");
		return 0;
	}

	client.fd = socket(	client.info->ai_family,
				client.info->ai_socktype,
				client.info->ai_protocol);

	if (client.fd == -1) {
		perror("Socket");
	} else {
		if (connect(	client.fd,
				client.info->ai_addr,
				client.info->ai_addrlen) == -1) {
			perror("Connect");
		} else {
			if (send(client.fd, &smsg, sizeof(smsg), 0) == -1)
				perror("Send");

			if (recv(client.fd, &rmsg, sizeof(rmsg), 0) == -1)
				perror("Recv");

			printf("%s\n", rmsg);
		}

		if (close(client.fd) != 0)
			perror("Close");
	}

	freeaddrinfo(client.info);

	return 0;
}
