/*
 * redirect_out.c   :   check for >
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT 0

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_out(char **myArgv)
{
	int i = 0;
	int fd;
	int is_found = 0;

	/* search forward for >
	 * Fill in code. */
	while (myArgv[i] != NULL)
	{
		if (strcmp(myArgv[i], ">") == 0)
		{
			is_found = 1;
			break;
		}
		i++;
	}
	if (!is_found)
		return -1;

	if (myArgv[i])
	{ /* found ">" in vector. */

		/* 1) Open file.
		 * 2) Redirect to use it for output.
		 * 3) Cleanup / close unneeded file descriptors.
		 * 4) Remove the ">" and the filename from myArgv.
		 *
		 * Fill in code. */
		// 1) Open file.
		if ((fd = open(myArgv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
		{
			perror("open");
			return -1;
		}
		// 2) Redirect to use it for output.
		if (dup2(fd, STD_OUTPUT) == -1)
		{
			perror("dup2");
			return -1;
		}
		// 3) Cleanup / close unneeded file descriptors.
		if (close(fd) == -1)
		{
			perror("close");
			return -1;
		}
		// 4) Remove the ">" and the filename from myArgv.
		myArgv[i] = NULL;
	}
	return 0;
}
