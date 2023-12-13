#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close

#define SOCKET_PATH "/tmp/example_socket"

int main()
{
    struct sockaddr_un server, client;
    int sd;
    char buffer[1024];
    int is_first_connect = 1;

    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, SOCKET_PATH);

    unlink(server.sun_path); // remove the socket file if it exists

    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(sd, 5) == -1)
    {
        perror("listen");
        exit(1);
    }
    printf("Server is listening...\n");

    int len = sizeof(client), comm;

    while (comm = accept(sd, (struct sockaddr *)&client, &len))
    {
        if (comm == -1)
        {
            perror("accept");
            // exit(1);
            continue;
        }
        if (is_first_connect)
        {
            printf("Client connected.\n");
            is_first_connect = 0;
        }

        // receive data from client
        int n = recv(comm, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            if (n == 0)
                printf("Client disconnected.\n");
            else if (n == -1)
                perror("recv");
            close(comm);
            continue;
        }

        printf("Received: %s", buffer);

        // send response to client
        char reply[] = "Message received.\n";
        if (send(comm, reply, sizeof(reply), 0) == -1)
        {
            perror("send");
            exit(1);
        }

        // if client sends "exit", then exit
        if (strcmp(buffer, "exit\n") == 0)
            break;
    }

    unlink(SOCKET_PATH);
    close(comm);
    close(sd);

    return 0;
}