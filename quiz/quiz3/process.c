#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include "stack.h" /* Stack function definitions. */
// #include <string.h>

data *globalStack;

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

    fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (ftruncate(fd, (off_t)sizeof(data)) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    globalStack = (data *)mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);

    if (globalStack == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    close(fd);

    pthread_mutexattr_t mutex_attributes;
    if (pthread_mutexattr_init(&mutex_attributes))
    {
        perror("pthread_mutexattr_init");
        exit(1);
    }

    if (pthread_mutexattr_setpshared(&mutex_attributes, PTHREAD_PROCESS_SHARED))
    {
        perror("pthread_mutexattr_setpshared");
        exit(1);
    }

    if (pthread_mutex_init(&globalStack->sharedMutex, &mutex_attributes))
    {
        perror("pthread_mutex_init");
        exit(1);
    }

    pthread_condattr_t condAttr;
    pthread_condattr_init(&condAttr);
    pthread_condattr_setpshared(&condAttr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&globalStack->sharedCond, &condAttr);
    pthread_condattr_destroy(&condAttr);

    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        int random = rand() % 2;
        int num = rand() % 100;
        switch (pids[i] = fork())
        {
        case -1:
            perror("fork");
            exit(1);

        case 0:
            // child process
            pid_t pid = getpid();
            printf("pid: %d\n", pid);
            if (random == 0)
                push(globalStack, num);
            else
                pop(globalStack);

            exit(0);
        default:
            // parent process
            wait(NULL);
            break;
        }
    }
    return 0;
}