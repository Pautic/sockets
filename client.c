#include <sys/socket.h>		/* socket(2) */
#include <netinet/in.h>		/* ip addr */
#include <arpa/inet.h>		/* htons(uint16_t hostshort) port -> network order */
#include <unistd.h>		/* close(2) */
#include <stdio.h>		/* perror */
#include <rpc/types.h>		/* INADDR_LOOPBACK ? */
#include <netdb.h>		/* struct addrinfo */

int main(int argc, char *argv[])
{
	struct addrinfo hints;
	struct addrinfo *serinfo;
	int clifd;
	char msg[] = "Hello world.";

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	hints.ai_flags = 0;

	if (getaddrinfo("127.0.0.1", "5000", &hints, &serinfo) != 0)
		perror("Getaddrinfo");

	clifd = socket(serinfo->ai_family, serinfo->ai_socktype,
				serinfo->ai_protocol);
	if (clifd == -1)
		perror("Socket");

	if (connect(clifd, serinfo->ai_addr, serinfo->ai_addrlen) == -1)
		perror("Connect");

	if (send(clifd, &msg, sizeof(msg), 0) == -1)
		perror("Send");

	if (close(clifd) != 0)
		perror("Close");
	freeaddrinfo(serinfo);

	return 0;
}
