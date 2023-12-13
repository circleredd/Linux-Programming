#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "errExit.h"

int main(int argc, char **argv)
{
    int source_fd, target_fd;
    ssize_t num_read;
    char buffer[BUFSIZ];

    // Open the source file for reading
    if ((source_fd = open(argv[1], O_RDONLY)) == -1)
        errexit("Failed to open the source file !");

    // Open or create the target file for writing
    if ((target_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
        errexit("Failed to open the target file !");

    while ((num_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        // Write the buffer to the target file
        if (write(target_fd, buffer, num_read) != num_read)
            errexit("Failed to Write !");
    }

    if (num_read == -1)
        errexit("Failed to Read !");

    close(source_fd);
    close(target_fd);
    return 0;
}