/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>	// for open
#include <unistd.h> // for close
#include "dict.h"

int main(int argc, char **argv)
{
	struct sockaddr_un server, client;
	int sd, cd, n;
	Dictrec tryit;

	if (argc != 3)
	{
		fprintf(stderr, "Usage : %s <dictionary source>"
						"<Socket name>\n",
				argv[0]);
		exit(errno);
	}

	/* Setup socket.
	 * Fill in code. */
	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, argv[2]);
	unlink(server.sun_path);

	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("bind");
		exit(1);
	}

	if (listen(sd, 5) == -1)
	{
		perror("listen");
		exit(1);
	}
	printf("Server is listening...\n");

	/* Initialize address.
	 * Fill in code. */

	/* Name and activate the socket.
	 * Fill in code. */

	int len = sizeof(client);

	/* main loop : accept connection; fork a child to have dialogue */
	for (;;)
	{

		/* Wait for a connection.
		 * Fill in code. */

		// 等待client端連線
		if ((cd = accept(sd, (struct sockaddr *)&client, &len)) == -1)
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
			while ((n = recv(cd, &tryit, sizeof(Dictrec), 0)) > 0)
			{
				/* Lookup request. */
				switch (lookup(&tryit, argv[1]))
				{
				/* Write response back to client. */
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
				} /* end lookup switch */

			} /* end of client dialog */

			/* Terminate child process.  It is done. */
			exit(0);

		/* Parent continues here. */
		default:
			close(cd);
			break;
		} /* end fork switch */
		  // break;
	}	  /* end forever loop */
} /* end main */
