/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>	// for open
#include <unistd.h> // for close
#include <string.h>

#include "dict.h"

int main(int argc, char **argv)
{
	static struct sockaddr_in server;
	int sockfd, cd, siz;
	Dictrec dr, *tryit = &dr;

	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s <datafile>\n", argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		DIE("socket");

	/* Initialize address.
	 * Fill in code. */
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // any address on this machine can access this socket

	/* Name and activate the socket.
	 * Fill in code. */
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
		DIE("bind");

	for (;;)
	{ /* await client packet; respond immediately */

		siz = sizeof(server); /* siz must be non-zero */

		/* Wait for a request.
		 * Fill in code. */

		while (recvfrom(sockfd, tryit, sizeof(Dictrec), 0, (struct sockaddr *)&server, &siz) > 0)
		{
			/* Lookup request and respond to user. */
			switch (lookup(tryit, argv[1]))
			{
			case FOUND:
				/* Send response.
				 * Fill in code. */
				if (sendto(sockfd, tryit, sizeof(Dictrec), 0, (struct sockaddr *)&server, sizeof(server)) == -1)
					DIE("sendto");
				break;

			case NOTFOUND:
				/* Send response.
				 * Fill in code. */
				strcpy(dr.text, "XXXX");
				if (sendto(sockfd, tryit, sizeof(Dictrec), 0, (struct sockaddr *)&server, sizeof(server)) == -1)
					DIE("sendto");
				break;

			case UNAVAIL:
				DIE(argv[1]);
			} /* end lookup switch */
		}	  /* end while */
	}		  /* end forever loop */
} /* end main */
