#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "errExit.h"

// ./tee -a file1
// ./tee -a file1 < file2
int main(int argc, char **argv)
{
    int fd;
    int opt = 0;
    char buffer[BUFSIZ];

    // Parse options
    if ((opt = getopt(argc, argv, "a")) == -1)
        errexit("Wrong option!");

    if (argc == 3)
    {
        if ((fd = open(argv[2], O_RDWR | O_APPEND)) == -1)
            errexit("Fail to open %s!", argv[2]);

        ssize_t bytes_read;

        while ((bytes_read = read(STDIN_FILENO, buffer, BUFSIZ)) > 0)
        {
            // write to std output
            write(STDOUT_FILENO, buffer, bytes_read);

            // write to file
            if (write(fd, buffer, bytes_read) == -1)
            {
                errexit("Fail to Write!");
            }
        }

        if (bytes_read == -1)
        {
            errexit("Fail to Read!");
        }
    }
    else
        errexit("Usage: ./tee -a file1 < file2 | ./tee -a file1");

    if (close(fd) == -1)
        errexit("Fail to close!");

    printf("\nSuccess !\n");
    return 0;
}