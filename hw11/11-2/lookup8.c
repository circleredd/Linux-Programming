/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>	// for open
#include <unistd.h> // for close

#include "dict.h"

int lookup(Dictrec *sought, const char *resource)
{
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time)
	{ /* connect to socket ; resource is server name */
		first_time = 0;

		/* Look up name of host serving directory resource.
		 * Fill in code. */
		if ((host = gethostbyname(resource)) == NULL)
			DIE("gethostbyname");

		/* Set up destination address. */
		/* Fill in code. */
		memset((char *)&server, '\0', sizeof(server)); // init sockaddr_in structure

		server.sin_family = AF_INET;
		server.sin_port = htons(PORT);
		memcpy((char *)&server.sin_addr, host->h_addr_list[0], host->h_length); // copy host address to server.sin_addr

		/* Allocate socket.
		 * Fill in code. */

		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			DIE("socket");

		/* Connect to the server.
		 * Fill in code. */
		if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
			DIE("connect");
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	if (send(sockfd, sought, sizeof(Dictrec), 0) == -1)
		DIE("send query to server");

	if ((recv(sockfd, sought, sizeof(Dictrec), 0)) == -1)
		DIE("client recv response");

	if (strcmp(sought->text, "XXXX") != 0)
	{
		return FOUND;
	}

	return NOTFOUND;
}
