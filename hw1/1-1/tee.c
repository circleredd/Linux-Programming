#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "errExit.h"

// ./tee -a file1
// ./tee -a file1 file2
int main(int argc, char **argv)
{
    int fd, fd2;
    ssize_t numRead, numWritten;
    int opt = 0;

    // Parse options
    if ((opt = getopt(argc, argv, "a")) == -1)
        errexit("Wrong option!");

    if (argc == 3)
    {
        if ((fd = open(argv[2], O_RDWR | O_APPEND)) == -1)
            errexit("Fail to open %s!", argv[2]);
        char buffer[BUFSIZ];
        ssize_t bytes_read;

        while ((bytes_read = read(STDIN_FILENO, buffer, BUFSIZ)) > 0)
        {
            // write to std output
            write(STDOUT_FILENO, buffer, bytes_read);

            // write to file
            if (write(fd, buffer, bytes_read) == -1)
            {
                perror("write");
                close(fd);
                exit(1);
            }
        }

        if (bytes_read == -1)
        {
            perror("read");
            close(fd);
            exit(1);
        }
    }

    // check if number of parameters fit the format
    if (argc != 4)
        errexit("Usage: %s -a file1 file2 | %s -a file1", argv[0], argv[0]);

    // open the dest file
    if ((fd = open(argv[2], O_RDWR | O_CREAT | O_APPEND)) == -1)
        errexit("Fail to open %s!", argv[2]);

    // open the input file
    if ((fd2 = open(argv[3], O_RDWR)) == -1)
        errexit("Fail to open %s!", argv[3]);
    int fileSize = lseek(fd2, 0, SEEK_END);
    lseek(fd2, 0, SEEK_SET);

    // read the input file to buf
    char buf[fileSize];
    if ((numRead = read(fd2, buf, fileSize)) == -1)
        errexit("Fail to read the input file!");

    // write the dest file
    if ((numWritten = write(fd, buf, fileSize)) == -1)
        errexit("Fail to write!");

    if (close(fd) == -1 || close(fd2) == -1)
        errexit("Fail to close!");

    printf("Success\n");
    return 0;
}