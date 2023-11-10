#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/server_fifo"

int main(int argc, char *argv[])
{
    int clientFd;
    int shouldOpen = argc > 1; // If there's an argument, client will open FIFO for reading.

    if (argc <= 1)
    {
        fputs("Usage: ./client /tmp/server_fifo\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (shouldOpen)
    {
        // Open the server's FIFO for reading to simulate a well-behaved client
        clientFd = open(SERVER_FIFO, O_WRONLY);
        if (clientFd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        write(clientFd, "Client has opened FIFO for reading.", 36);

        printf("Client has opened FIFO for reading.\n");
    }
    else
    {
        printf("Client is misbehaving and will not open FIFO for reading.\n");
    }

    if (shouldOpen)
    {
        close(clientFd);
    }

    return 0;
}
