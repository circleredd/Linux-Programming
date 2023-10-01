/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char **parse(char *line)
{

	static char delim[] = " \t\n"; /* SPACE or TAB or NL */
	int count = 0;
	char *token;
	char **newArgv;

	/* Nothing entered. */
	if (line == NULL)
	{
		return NULL;
	}

	/* Init strtok with commandline, then get first token.
	 * Return NULL if no tokens in line.
	 *
	 * Fill in code.
	 */
	token = strtok(line, delim);
	if (!token)
		return NULL;

	/* Create array with room for first token.
	 *
	 * Fill in code.
	 */
	newArgv = malloc(1 * sizeof(char *));
	newArgv[0] = token;

	/* While there are more tokens...
	 *
	 *  - Get next token.
	 *	- Resize array.
	 *  - Give token its own memory, then install it.
	 *
	 * Fill in code.
	 */

	int tokenCount = 1;

	while (token)
	{
		token = strtok(NULL, delim);
		if (!token)
			break;
		newArgv = realloc(newArgv, (tokenCount + 1) * sizeof(char *));
		newArgv[tokenCount++] = token;
	}

	for (int i = 0; i < tokenCount; i++)
	{
		printf("[%d] : %s\n", i, newArgv[i]);
	}

	/* Null terminate the array and return it.
	 *
	 * Fill in code.
	 */

	return newArgv;
}

/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv)
{

	int i = 0;

	/* Free each string hanging off the array.
	 * Free the oldArgv array itself.
	 *
	 * Fill in code.
	 */

	while (oldArgv[i])
	{
		free(i);
		i++;
	}
	free(oldArgv);

	return;
}