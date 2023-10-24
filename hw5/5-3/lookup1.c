/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include "dict.h"

int lookup(Dictrec *sought, const char *resource)
{
	Dictrec dr;
	static FILE *in;
	static int first_time = 1;

	if (first_time)
	{
		first_time = 0;
		/* open up the file
		 *
		 * Fill in code. */
		in = fopen(resource, "r");
	}

	/* read from top of file, looking for match
	 *
	 * Fill in code. */

	// 將指針指向開頭
	rewind(in);

	// 將file中的內容讀進dr
	while (fread(&dr, sizeof(Dictrec), 1, in))
	{
		/* Fill in code. */
		if (strcmp(dr.word, sought->word) == 0)
		{
			strcpy(sought->text, dr.text);
			return FOUND;
		}
	}
	fclose(in);

	return NOTFOUND;
}
