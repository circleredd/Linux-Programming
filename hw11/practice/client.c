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
    int sd;
    struct sockaddr_un server;

    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("socket");
        exit(1);
    }

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, SOCKET_PATH);

    if ((connect(sd, (struct sockaddr *)&server, sizeof(server))) == -1)
    {
        perror("connect");
        exit(1);
    }
    // send data to server
    char buffer[1024];
    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    send(sd, buffer, sizeof(buffer), 0);

    // receive data from server
    recv(sd, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);

    return 0;
}