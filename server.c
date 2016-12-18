#include <sys/socket.h>		/* socket() */
#include <netinet/in.h>		/* struct sockaddr_in & struct in_addr ? */
#include <arpa/inet.h>		/* htons() & htonl() */
#include <unistd.h>		/* close() */
#include <rpc/types.h>		/* INADDR_LOOPBACK ? */
#include <stdio.h>		/* perror */

#include "sockets.h"

struct socksvr {
	int fd;
	struct sockaddr_in addr;
};

int setup_socket(struct sockaddr_in *addr, size_t size)
{
	/* Create endpoint, get file descriptor */
	int fd = socket(addr->sin_family, SOCK_STREAM, 0);
	if (fd == -1)
		perror("Socket");

	/* Assign name (address) to socket */
	if (bind(fd, (struct sockaddr *) addr, size) != 0)
		perror("Bind");

	/* Listen on socket */
	if (listen(fd, 1) != 0)
		perror("Listen");

	return fd;
}

int main(int argc, char *argv[])
{
	/* Port, get user input else default port */
	unsigned int port = (argc == 2) ? get_port(argv) : defport;

	/* Messages */
	char rmsg[16];

	/* file descriptors & address */
	struct socksvr svr;
	struct socksvr clt;

	svr.addr.sin_family		= addrfam;
	svr.addr.sin_port		= htons(port);
	svr.addr.sin_addr.s_addr	= htonl(INADDR_LOOPBACK); /* localhost */
	socklen_t len			= sizeof(struct sockaddr_in);

	/* Create, bind and listen */
	svr.fd = setup_socket(&svr.addr, sizeof(svr.addr));
	if (svr.fd == -1)
		return 0;

	clt.fd = accept(svr.fd, (struct sockaddr *)&clt.addr, &len);
	if (clt.fd == -1)
		perror("Accept");

	/* read, write */
	if (read(clt.fd, &rmsg, sizeof(rmsg)) == -1)
		perror("Recv");

	rmsg[15] = '\0';
	printf("%s\n", rmsg);

	if (send(clt.fd, &rmsg, sizeof(rmsg), 0) == -1)
		perror("Send");

	/* Close file descriptors */
	if (close(clt.fd) != 0)
		perror("Close client");
	if (close(svr.fd) != 0)
		perror("Close server");

	return 0;
}
