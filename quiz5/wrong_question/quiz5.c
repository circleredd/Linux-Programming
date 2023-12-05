
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <aio.h>
#include <error.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAXLINE 4096 /* max line length */

static void sig_pipe(int); /* our signal handler */

int main(void)
{
	int n, fd1[2], fd2[2];
	pid_t pid;
	char line[MAXLINE];
	FILE *parent_out, *child_in;
	char message;
	// int fd;
	// int *map;

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
	{
		perror("signal error");
		exit(1);
	}

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		perror("pipe error");

	// fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
	// if (ftruncate(fd, (off_t)sizeof(int)) == -1)
	// {
	// 	perror("ftruncate");
	// 	exit(1);
	// }
	// map = (int *)mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	// map[0] = 1;

	switch (pid = fork())
	{
	case -1:
		perror("fork error");
		break;
	case 0:
		/* child */
		close(fd1[1]);
		close(fd2[0]);
		FILE *parent_in, *child_out;

		parent_in = fdopen(fd1[0], "r");
		child_out = fdopen(fd2[1], "w");
		if (fread(&message, sizeof(char), 1, parent_in) != 1)
		{
			perror("child read error from pipe");
			exit(1);
		}

		if (message == 'c')
		{
			printf("child read c\n");
			message = 'p';
			// map[0] = 0;
		}
		else
			printf("WAIT_CHILD: incorrect data\n");
		if (fwrite(&message, sizeof(char), 1, child_out) != 1)
		{
			perror("child write error to pipe");
			exit(1);
		}

		exit(0);
		break;

	default:
		/* parent */
		close(fd1[0]);
		close(fd2[1]);
		parent_out = fdopen(fd1[1], "w");
		child_in = fdopen(fd2[0], "r");
		printf("please enter a character:\n");
		char message2 = fgetc(stdin);

		if (fwrite(&message2, sizeof(char), 1, parent_out) != 1)
		{
			perror("parent write error to pipe");
			exit(1);
		}
		fclose(parent_out);

		wait(NULL);
		if (fread(&message, sizeof(char), 1, child_in) != 1)
		{
			perror("parent read error from pipe");
			exit(1);
		}

		if (message == 'p')
			printf("parent read p\n");
		else
			printf("WAIT_PARENT: incorrect data\n");

		break;
	}
}

static void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}
