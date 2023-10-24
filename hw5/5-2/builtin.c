/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include "shell.h"
#include <errno.h>
#include <sys/types.h>

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(char **argv);  /* "builtin" command tells whether a command is builtin or not. */
static void bi_cd(char **argv);		  /* "cd" command. */
static void bi_echo(char **argv);	  /* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_hostname(char **argv); /* "hostname" command. */
static void bi_id(char **argv);		  /* "id" command shows user and group of this process. */
static void bi_pwd(char **argv);	  /* "pwd" command. */
static void bi_quit(char **argv);	  /* quit/exit/logout/bye command. */

/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd
{
	char *keyword;			/* When this field is argv[0] ... */
	void (*do_it)(char **); /* ... this function is executed. */
} inbuilts[] = {
	{"builtin", bi_builtin}, /* List of (argv[0], function) pairs. */

	/* Fill in code. */
	{"echo", bi_echo},
	{"quit", bi_quit},
	{"exit", bi_quit},
	{"bye", bi_quit},
	{"logout", bi_quit},
	{"cd", bi_cd},
	{"pwd", bi_pwd},
	{"id", bi_id},
	{"hostname", bi_hostname},
	{NULL, NULL} /* NULL terminated. */
};

static void bi_builtin(char **argv)
{
	/* Fill in code. */

	// 如果builtin後面沒有傳入指令
	if (argv[1] == NULL)
	{
		printf("Usage: builtin [command]\n");
		return;
	}
	if (is_builtin(argv[1]))
	{
		printf("%s is a builtin feature.\n", argv[1]);
	}
	else
	{
		printf("%s is not a builtin feature.\n", argv[1]);
	}
}

static void bi_cd(char **argv)
{
	/* Fill in code. */
	if (argv[1] == NULL)
	{
		fprintf(stderr, "cd: missing argument\n");
	}
	else
	{
		if (chdir(argv[1]) != 0)
		{
			perror("cd");
		}
	}
}

static void bi_echo(char **argv)
{
	/* Fill in code. */

	int line;
	int count = 0;
	if ((strcmp(argv[1], "-n") == 0) && isdigit(*argv[2]))
	{
		line = atoi(argv[2]);
		printf("%s\n", argv[line + 2]);
	}
	else
	{
		count = 1;
		while (argv[count])
		{
			printf("%s ", argv[count++]);
		}
		printf("\n");
	}
}

static void bi_hostname(char **argv)
{
	/* Fill in code. */
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == 0)
	{
		printf("hostname: %s\n", hostname);
	}
	else
	{
		perror("gethostname");
		return 1;
	}
}

static void bi_id(char **argv)
{
	/* Fill in code. */
	uid_t user_id = getuid();
	gid_t group_id = getgid();

	printf("User ID: %d, Group ID: %d\n", user_id, group_id);
}

static void bi_pwd(char **argv)
{
	/* Fill in code. */
	char *current_dir = NULL;
	size_t size = 256; // 初始buffer大小

	while (1)
	{
		current_dir = (char *)malloc(size);
		// allocate 失敗
		if (current_dir == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		if (getcwd(current_dir, size) != NULL)
		{
			printf("%s\n", current_dir);
			free(current_dir);
			break;
		}
		else
		{
			// 如果buffer不夠大，getcwd會回傳NULL並設定errno為ERANGE
			if (errno == ERANGE)
			{
				free(current_dir);
				size *= 2; // 增加buffer大小
			}
			else
			{
				perror("getcwd");
				free(current_dir);
				exit(EXIT_FAILURE);
			}
		}
	}
	return;
}

static void bi_quit(char **argv)
{
	exit(0);
}

/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd *this; /* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd)
{
	struct cmd *tableCommand;

	for (tableCommand = inbuilts; tableCommand->keyword != NULL; tableCommand++)
		if (strcmp(tableCommand->keyword, cmd) == 0)
		{
			this = tableCommand;
			return 1;
		}
	return 0;
}

/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv)
{
	this->do_it(argv);
}
