/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
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
	{ /* Set up server address & create local UDP socket */
		first_time = 0;
		if ((host = gethostbyname(resource)) == NULL)
			DIE("gethostbyname");

		/* Set up destination address.
		 * Fill in code. */
		memset((char *)&server, '\0', sizeof(server)); // init sockaddr_in structure

		/* Allocate a socket.
		 * Fill in code. */
		server.sin_family = AF_INET;
		server.sin_port = htons(PORT);
		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
			DIE("socket");
		memcpy((char *)&server.sin_addr, host->h_addr_list[0], host->h_length); // copy host address to server.sin_addr
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	socklen_t addr_size = sizeof(server);

	if (sendto(sockfd, sought, sizeof(Dictrec), 0, (struct sockaddr *)&server, addr_size) == -1)
		DIE("sendto");

	if (recvfrom(sockfd, sought, sizeof(Dictrec), 0, NULL, NULL) == -1)
		DIE("recvfrom");

	if (strcmp(sought->text, "XXXX") != 0)
	{
		return FOUND;
	}

	return NOTFOUND;
}
