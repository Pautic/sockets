#include <stdlib.h>		/* strtoul */
#include <sys/socket.h>		/* socket() */
#include <netinet/in.h>		/* struct sockaddr_in & struct in_addr ? */
#include <unistd.h>		/* close() */
#include <netdb.h>		/* struct addrinfo */
#include <stdio.h>		/* perror */
#include <string.h>		/* memset */

#include "sockets.h"

int main(int argc, char *argv[])
{
	/* Message */
	char rmsg[16];
	char smsg[] = "Hello server";

	/* Port */
	char b = 6;
	char port[b];
	unsigned int p = defport;
	if (argc == 2) {
		unsigned int tmp = strtoul(argv[1], NULL, 10);
		if ((tmp > 1024) && (tmp < 65537))
			p = tmp;
	}
	snprintf(port, b, "%u", p);

	/* File descriptor & addresses */
	int cfd;
	struct addrinfo hints;
	struct addrinfo *info;

	/*
	** "All the other fields in the structure pointed to by hints must
	** contain either 0 or a null pointer, as appropriate."
	*/
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_NUMERICSERV;

	/*
	** "If node is NULL, then the network address
	** will be set to the loopback interface address"
	*/
	if (getaddrinfo(NULL, port, &hints, &info) != 0) {
		perror("Getaddrinfo");
		return 0;
	}

	cfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

	if (cfd == -1) {
		perror("Socket");
		freeaddrinfo(info); /* Find better way to deal with freeaddrinfo */
		return 0;
	}

	if (connect(cfd, info->ai_addr, info->ai_addrlen) == -1) {
		perror("Connect");
	} else {
		if (send(cfd, &smsg, sizeof(smsg), 0) == -1)
			perror("Send");

		if (recv(cfd, &rmsg, sizeof(rmsg), 0) == -1)
			perror("Recv");

		printf("%s\n", rmsg);
	}

	if (close(cfd) != 0)
		perror("Close");
	freeaddrinfo(info);

	return 0;
}
