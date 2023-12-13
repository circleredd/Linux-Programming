#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct data
{
    int seg;
    int data;
} data;

void generator(data *data, int data_num)
{
    for (int i = 0; i < data_num; i++)
    {
        data[i].seg = i;
        data[i].data = rand() % 9 + 1;
    }
    return;
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]), k = atoi(argv[2]), data_num = atoi(argv[3]);
    int sum = 0;
    int fd;
    char *shared_data;

    data data[data_num];

    // 初始化random seed
    srand(time(NULL));

    // 產生data
    generator(data, data_num);

    fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (ftruncate(fd, (off_t)(sizeof(data) * k)) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    // 透過mmap傳送資料給多個child process
    shared_data = mmap(NULL, sizeof(data) * data_num, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);

    if (shared_data == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    close(fd);

    for (int i = 0; i < data_num; i++)
    {
        shared_data[i] = data[i].data;
    }

    pid_t pids[n];
    // 創建n個process
    for (int i = 0; i < n; i++)
    {
        switch (pids[i] = fork())
        {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            // child process
            pid_t pid = getpid();
            int child_sum = 0;
            for (int j = 0; j < data_num; j++)
            {
                child_sum += shared_data[j];
            }

            printf("Child process %d: %d\n", i + 1, pid);
            printf("child sum = %d\n", child_sum);
            exit(0);
        default:
            // parent process
            wait(NULL);
        }
    }
    printf("Parent process %d\n", getpid());

    for (int i = 0; i < data_num; i++)
    {
        sum += data[i].data;
    }
    printf("parent sum = %d\n", sum);

    return 0;
}
