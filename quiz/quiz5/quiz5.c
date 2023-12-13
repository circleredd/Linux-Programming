// #include "apue.h"
// Manually include the C standard library.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <error.h>
#include <aio.h>

#define MAXLINE 4096 /* max line length */

static void sig_pipe(int); /* our signal handler */

int main(void)
{
	int n, fd1[2], fd2[2];
	pid_t pid;
	char line[MAXLINE];

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
		perror("signal error");

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		perror("pipe error");

	if ((pid = fork()) < 0)
	{
		perror("fork error");
	}
	else if (pid > 0)
	{ /* parent */
		close(fd1[0]);
		close(fd2[1]);
		FILE *parent_out, *parent_in;
		parent_out = fdopen(fd1[1], "w");
		parent_in = fdopen(fd2[0], "r");

		while (fgets(line, MAXLINE, stdin) != NULL)
		{
			n = strlen(line);

			if (fwrite(line, sizeof(char), n, parent_out) != n)
				perror("write error to pipe");

			fflush(parent_out);

			// if ((n = fread(line, sizeof(char), MAXLINE, parent_in)) < 0)
			// 	perror("read error from pipe");
			if (fgets(line, MAXLINE, parent_in) == NULL)
			{
				perror("read error from pipe");
			}

			if (n == 0)
			{
				perror("child closed pipe");
				break;
			}
			line[n] = '\0'; /* null terminate */
			if (fputs(line, stdout) == EOF)
				perror("fputs error");
		}

		if (ferror(stdin))
			perror("fgets error on stdin");
		exit(0);
	}
	else
	{ /* child */
		close(fd1[1]);
		close(fd2[0]);
		FILE *child_in, *child_out;
		child_in = fdopen(fd1[0], "r");
		child_out = fdopen(fd2[1], "w");
		// close(fd1[0]);
		// close(fd2[1]);

		if (fd1[0] != STDIN_FILENO)
		{
			if (dup2(fileno(child_in), STDIN_FILENO) != STDIN_FILENO)
				perror("dup2 error to stdin");
		}

		if (fd2[1] != STDOUT_FILENO)
		{
			if (dup2(fileno(child_out), STDOUT_FILENO) != STDOUT_FILENO)
				perror("dup2 error to stdout");
		}

		if (execl("./add2", "add2", (char *)0) < 0)
			perror("execl error");
	}
	exit(0);
}

static void
sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}
