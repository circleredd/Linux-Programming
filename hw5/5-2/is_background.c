/*
 * is_background.c :  check for & at end
 */

#include <stdio.h>
#include "shell.h"

int is_background(char **myArgv)
{

	if (*myArgv == NULL)
		return 0;

	/* Look for "&" in myArgv, and process it.
	 *
	 *	- Return TRUE if found.
	 *	- Return FALSE if not found.
	 *
	 * Fill in code.
	 */
	char **index = myArgv;
	while (*index != NULL)
	{
		// 檢查字串中有沒有&
		if (strcmp(*index, "&") == 0)
		{
			return 1;
		}
		index++;
	}
}