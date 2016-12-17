#include <stdlib.h>		/* strtoul */
#include <sys/socket.h>		/* socket() */
#include <netinet/in.h>		/* struct sockaddr_in & struct in_addr ? */
#include <arpa/inet.h>		/* htons() & htonl() */
#include <unistd.h>		/* close() */
#include <rpc/types.h>		/* INADDR_LOOPBACK ? */
#include <stdio.h>		/* perror */

#include "sockets.h"

int main(int argc, char *argv[])
{
	/* Message */
	char rmsg[16];
	char smsg[] = "Hello client";

	/* Port */
	unsigned int port = defport;
	if (argc == 2) {
		unsigned int tmp = strtoul(argv[1], NULL, 10);
		port = (tmp > 1024) ? tmp : port;
	}

	/* file descriptors & address */
	int sfd, cfd;
	struct sockaddr_in saddr, caddr;

	saddr.sin_family	= AF_INET;
	saddr.sin_port		= htons(port);
	saddr.sin_addr.s_addr	= htonl(INADDR_LOOPBACK);
	socklen_t caddrlen	= sizeof(caddr);

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		perror("Socket");
		return 0;
	}

	if (bind(sfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
		perror("Bind");

	if (listen(sfd, 4) != 0)
		perror("Listen");

	cfd = accept(sfd, (struct sockaddr *) &caddr, &caddrlen);
	if (cfd == -1)
		perror("Accept");

	if (recv(cfd, &rmsg, sizeof(rmsg), 0) == -1)
		perror("Recv");

	printf("%s\n", rmsg);

	if (send(cfd, &smsg, sizeof(smsg), 0) == -1)
		perror("Send");

	if (close(cfd) != 0)
		perror("Close client");
	if (close(sfd) != 0)
		perror("Close server");

	return 0;
}
