#include <sys/socket.h>		/* socket(2) */
#include <netinet/in.h>		/* ip addr */
#include <arpa/inet.h>		/* htons(uint16_t hostshort) port -> network order */
#include <unistd.h>		/* close(2) */
#include <stdio.h>		/* perror */
#include <rpc/types.h>		/* INADDR_LOOPBACK ? */

int main(int argc, char *argv[])
{
	char msg[16];
	int serfd, clifd;
	struct sockaddr_in seraddr, cliaddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(5000U);
	/* INADDR_LOOPBACK is not network order, unlike INADDR_ANY */
	seraddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	socklen_t cliaddrlen = sizeof(cliaddr);

	serfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serfd == -1)
		perror("Socket");

	if (bind(serfd, (struct sockaddr *) &seraddr, sizeof(seraddr)) != 0)
		perror("Bind");

	if (listen(serfd, 4) != 0)
		perror("Listen");

	clifd = accept(serfd, (struct sockaddr *) &cliaddr, &cliaddrlen);
	if (clifd == -1)
		perror("Accept");

	if (recv(clifd, &msg, sizeof(msg), 0) == -1)
		perror("Recv");

	printf("%s\n", msg);

	if (close(clifd) != 0)
		perror("Close client");
	if (close(serfd) != 0)
		perror("Close server");

	return 0;
}
