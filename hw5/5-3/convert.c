/*
 * convert.c : take a file in the form
 *  word1
 *  multiline definition of word1
 *  stretching over several lines,
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv)
{
	FILE *in;
	FILE *out; /* defaults */
	char line[BIGLINE];
	static Dictrec dr;
	// static Dictrec blank;

	/* If args are supplied, argv[1] is for input, argv[2] for output */
	if (argc == 3)
	{
		if ((in = fopen(argv[1], "r")) == NULL)
		{
			DIE(argv[1]);
		}
		if ((out = fopen(argv[2], "w")) == NULL)
		{
			DIE(argv[2]);
		}
	}
	else
	{
		printf("Usage: convert [input file] [output file].\n");
		return -1;
	}

	/* Main reading loop : read word first, then definition into dr */

	/* Loop through the whole file. */

	// while (!feof(in))
	// {

	// 	/* Create and fill in a new blank record.
	// 	 * First get a word and put it in the word field, then get the definition
	// 	 * and put it in the text field at the right offset.  Pad the unused chars
	// 	 * in both fields with nulls.
	// 	 */

	// 	/* Read word and put in record.  Truncate at the end of the "word" field.
	// 	 *
	// 	 * Fill in code. */
	// 	if (fgets(line, sizeof(line), in) != NULL)
	// 	{
	// 		line[strlen(line) - 1] = '\0';
	// 		strcpy(dr.word, line);
	// 	}
	// 	printf("word: %s\n", dr.word);

	// 	/* Read definition, line by line, and put in record.
	// 	 *
	// 	 * Fill in code. */
	// 	while (fgets(line, sizeof(line), in) != NULL)
	// 	{
	// 		if (strlen(line) == 2 && line[0] == '\n')
	// 			break;
	// 		strcpy(dr.text, line);
	// 		printf("text: %s\n", line);
	// 	}

	// 	return 0;

	// 	/* Write record out to file.
	// 	 *
	// 	 * Fill in code. */
	// }

	// 判斷是否為word的flag
	int is_word = 1;
	while (fgets(line, sizeof(line), in))
	{
		// 讀取word
		if (is_word)
		{
			// 將換行符號替換為空字元
			line[strcspn(line, "\n")] = '\0';
			strcpy(dr.word, line);
			is_word = 0; // 讀取完word，將flag設為0
		}

		if (line[0] == '\n')
		{
			// 遇到空行，讀取下一單字前寫入當前記錄到文件
			fwrite(&dr, sizeof(dr), 1, out);

			// 重置紀錄，並將is_word設為1
			memset(&dr, 0, sizeof(Dictrec));
			is_word = 1;
		}
		else
		{
			// 讀取text
			strcat(dr.text, line);
		}
	}

	fclose(in);
	fclose(out);
	return 0;
}
