/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
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
	static struct sockaddr_in server, client;
	int sd, cd, n;
	Dictrec tryit;

	if (argc != 2)
	{
		fprintf(stderr, "Usage : %s <datafile>\n", argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	sd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize address.
	 * Fill in code. */
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // any address on this machine can access this socket

	/* Name and activate the socket.
	 * Fill in code. */
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
		DIE("bind");

	if (listen(sd, 5) == -1)
		DIE("listen");

	int len = sizeof(client);

	/* main loop : accept connection; fork a child to have dialogue */
	for (;;)
	{

		/* Wait for a connection.
		 * Fill in code. */
		if (cd = accept(sd, (struct sockaddr *)&client, &len) == -1)
		{
			DIE("accept");
			continue;
		}

		/* Handle new client in a subprocess. */
		switch (fork())
		{
		case -1:
			DIE("fork");
		case 0:
			close(sd); /* Rendezvous socket is for parent only. */
			/* Get next request.
			 * Fill in code. */
			while (recv(cd, &tryit, sizeof(Dictrec), 0) > 0)
			{
				/* Lookup the word , handling the different cases appropriately */
				switch (lookup(&tryit, argv[1]))
				{
				/* Write response back to the client. */
				case FOUND:
					/* Fill in code. */
					if (send(cd, &tryit, sizeof(Dictrec), 0) == -1)
						DIE("send");
					break;

				case NOTFOUND:
					/* Fill in code. */
					strcpy(tryit.text, "XXXX");
					if (send(cd, &tryit, sizeof(Dictrec), 0) == -1)
						DIE("send");
					break;

				case UNAVAIL:
					DIE(argv[1]);
				}	 /* end lookup switch */
			}		 /* end of client dialog */
			exit(0); /* child does not want to be a parent */

		default:
			close(cd);
			break;
		} /* end fork switch */
	}	  /* end forever loop */
} /* end main */
