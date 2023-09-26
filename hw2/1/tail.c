#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include "errExit.h"
/*
./tail filename
./tail -n num filename
*/

int main(int argc, char **argv)
{
	int opt, numLines = 10; // opt and number of tail lines we want to fetch, by default is 10
	int fd;					// the opened file flag
	int fileArgvIndex = 1;	// the argv index of the inputfile, by default is 1
	long int numRead;		// number of char read from the file;
	char *buf = malloc(4096 * sizeof(char));

	if (argc == 3 && atoi(argv[1]) * -1 > 0)
	{
		numLines = atoi(argv[1]) * -1;
		fileArgvIndex = 2;
	}
	else if (argc == 2 || argc == 4)
	{
		while ((opt = getopt(argc, argv, "n:")) != -1)
		{
			switch (opt)
			{
			case 'n':
				numLines = atoi(optarg); // convert the optarg from string to int
				fileArgvIndex = 3;
				break;

			default:
				break;
			}
		}
	}
	else
	{
		errexit("Usage: %s filename or %s -n num filename or %s -num filename", argv[0], argv[0], argv[0]);
	}

	if ((fd = open(argv[fileArgvIndex], O_RDONLY)) == -1)
		errexit("Fail to open");

	if ((numRead = read(fd, buf, 1024)) == -1)
		errexit("Fail to read!");
	buf[numRead] = '\0';

	// To find the position we want to write to STDOUT by finding the number of '\n'
	long int lines = 0;
	long int cur = -1;
	for (int i = numRead - 1; i >= 0; i--)
	{
		if (buf[i] == '\n')
			lines++;
		else if (i > 0 && lines == numLines - 1 && buf[i - 1] == '\n')
		{
			cur = i;
			break;
		}
	}
	cur = (cur == -1) ? 0 : cur;

	if (write(STDOUT_FILENO, (buf + cur), numRead - cur) == -1)
		errexit("Fail to write!");

	if (close(fd) == -1)
		errexit("Fail to close the file!");

	free(buf);

	return 0;
}

// printf("numLines: %d\n", numLines);
// printf("cur: %ld\n", cur);