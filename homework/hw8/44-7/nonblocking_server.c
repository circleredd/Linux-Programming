#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/server_fifo"

int main()
{
    int serverFd, dummyFd;
    int req_num = 1;

    // Create a FIFO with read/write permissions for user
    umask(0);
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO in nonblocking mode
    serverFd = open(SERVER_FIFO, O_RDONLY | O_NONBLOCK);
    if (serverFd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // This dummy open prevents seeing EOF
    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if (dummyFd == -1)
    {
        perror("open dummy");
        close(serverFd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running. Waiting for client to open FIFO for reading...\n");

    // Attempt to read something from the FIFO
    for (;;)
    {
        char buffer[100];
        ssize_t numRead = read(serverFd, buffer, sizeof(buffer));

        if (numRead == -1)
        {
            if (errno == EAGAIN)
            {
                continue;
            }
            else
            {
                perror("read");
            }
        }
        else if (numRead == 0)
        {
            printf("End of file reached. Client has closed the FIFO.\n");
        }
        else
        {
            printf("Received data %d: %s\n", req_num, buffer);
            req_num++;
        }
    }
    close(serverFd);
    close(dummyFd);
    unlink(SERVER_FIFO);

    return 0;
}
