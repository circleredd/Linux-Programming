/*
 * pipe_command.c  :  deal with pipes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "shell.h"
#include <sys/wait.h>

#define STD_OUTPUT 1
#define STD_INPUT 0

void pipe_and_exec(char **myArgv)
{
	int pipe_argv_index = pipe_present(myArgv);
	int pipefds[2];
	char **left_argv;
	char **right_argv;

	switch (pipe_argv_index)
	{

	case -1: /* Pipe at beginning or at end of argv;  See pipe_present(). */
		fputs("Missing command next to pipe in commandline.\n", stderr);
		errno = EINVAL; /* Note this is NOT shell exit. */
		break;

	case 0: /* No pipe found in argv array or at end of argv array.
		See pipe_present().  Exec with whole given argv array. */
		execvp(myArgv[0], myArgv);
		perror("execvp");
		break;

	default: /* Pipe in the middle of argv array.  See pipe_present(). */

		/* Split arg vector into two where the pipe symbol was found.
		 * Terminate first half of vector.
		 *
		 * Fill in code. */
		// Terminate first half of vector.

		// 如果指令中有pipe符號，將pipe前及後的切成兩段
		left_argv = myArgv;
		right_argv = &myArgv[pipe_argv_index + 1];
		myArgv[pipe_argv_index] = NULL;

		/* Create a pipe to bridge the left and right halves of the vector.
		 *
		 * Fill in code. */
		if (pipe(pipefds) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		/* Create a new process for the right side of the pipe.
		 * (The left side is the running "parent".)
		 *
		 * Fill in code to replace the underline. */
		pid_t pid;
		switch (pid = fork())
		{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;

		/* Talking parent.  Remember this is a child forked from shell. */
		default:

			/* - Redirect output of "parent" through the pipe.
			 * - Don't need read side of pipe open.  Write side dup'ed to stdout.
			 * - Exec the left command.
			 *
			 * Fill in code. */

			// 父程序負責寫入pipe，所以關閉讀取端，並執行pipe左邊的指令
			close(pipefds[0]);

			dup2(pipefds[1], STDOUT_FILENO);
			close(pipefds[1]);

			execvp(left_argv[0], left_argv);
			perror("execvp");
			exit(EXIT_FAILURE);

			break;

		/* Listening child. */
		case 0:

			/* - Redirect input of "child" through pipe.
			 * - Don't need write side of pipe. Read side dup'ed to stdin.
			 * - Exec command on right side of pipe and recursively deal with other pipes
			 *
			 * Fill in code. */

			// 子程序負責讀取pipe，所以關閉寫入端，並執行pipe右邊的指令
			close(pipefds[1]);
			dup2(pipefds[0], STDIN_FILENO);
			close(pipefds[0]);
			pipe_and_exec(&myArgv[pipe_argv_index + 1]);
		}
		break;
	}

	return;
}
