#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void mycat(char *argv[])
{
    int fd;

    // read file with linux function open and read and write to stdout
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        fprintf(stderr, "fail to open %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0)
    {
        write(STDOUT_FILENO, buf, bytes_read);
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mycat(argv);

    return 0;
}