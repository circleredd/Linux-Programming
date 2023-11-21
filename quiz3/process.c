#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "stack.h" /* Stack function definitions. */

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: %s <n>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    pid_t pids[n];
    int fd;
    char *shared_data;

    fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
    // if (ftruncate(fd, (off_t)(sizeof(data) * k)) == -1)
    // {
    //     perror("ftruncate");
    //     exit(1);
    // }

    shared_data = mmap(NULL, sizeof(char) * STACK_SIZE + sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);

    if (shared_data == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    close(fd);

    for (int i = 0; i < n; i++)
    {
        char rand = 'a' + i;
        switch (pids[i] = fork())
        {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            // child process
            pid_t pid = getpid();
            printf("child process %d\n", pid);
            push(rand);
            memcpy(shared_data, buffer, sizeof(char) * STACK_SIZE);
            memcpy(shared_data + sizeof(char) * STACK_SIZE, &index, sizeof(int));

            exit(0);
        default:
            // parent process
            wait(NULL);
        }
    }
    return 0;
}