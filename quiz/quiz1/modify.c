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
#include <sys/time.h>

typedef struct data
{
    int seg;
    int data;
} Data;

void generator(Data *data, int data_num)
{
    for (int i = 0; i < data_num; i++)
    {
        data[i].seg = i;
        data[i].data = rand() % 9 + 1;
    }
    return;
}

volatile sig_atomic_t received = 0;
void sig_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        received = 1;
    }
}
int main(int argc, char *argv[])
{

    if (argc < 4)
    {
        printf("Usage: %s <N> <K> <data_num>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]), k = atoi(argv[2]), data_num = atoi(argv[3]);
    int sum = 0;
    int fd;
    Data *shared_data;
    Data data[k];
    int count = 0;

    // 初始化random seed
    srand(time(NULL));

    // 產生data
    generator(data, data_num);

    fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (ftruncate(fd, (off_t)(sizeof(data) * data_num)) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    // 透過mmap傳送資料給多個child process, 並將return type轉型為Data
    shared_data = (Data *)mmap(NULL, sizeof(data) * data_num, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);

    if (shared_data == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    close(fd);

    signal(SIGUSR1, sig_handler);

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

            // 子进程循环直到所有数据被处理
            while (count < data_num)
            {
                // 子进程等待信号
                while (!received)
                {
                    pause(); // Wait for SIGUSR1 to be received
                }

                // 子进程计算共享内存中的sum
                for (int j = 0; j < k && j < data_num; j++)
                {
                    child_sum += shared_data[j].data;
                }
                printf("Child process %d: %d\n", i + 1, getpid());
                printf("Child sum = %d\n", child_sum);

                // 重置信号接收标志
                received = 0;

                // 做完工作后，通知父进程
                kill(getppid(), SIGUSR2); // You need to handle SIGUSR2 in parent.
            }
            exit(0);
            // 子进程完成计算后退出
            // 子进程完成计算后退出
            // int child_sum = 0;
            // while (1)
            // {
            //     if (received == 1)
            //     {
            //         for (int j = 0; j < k; j++)
            //         {
            //             child_sum += shared_data[j].data;
            //         }
            //         printf("Child process %d: %d\n", i + 1, pid);
            //         printf("child sum = %d\n", child_sum);
            //     }
            //     received = 0;
            //     exit(0);
            // }

        default:
            // parent process
            while (count < data_num)
            {
                // 将下一个数据块写入共享内存
                for (int j = 0; j < k && count < data_num; j++, count++)
                {
                    shared_data[j] = data[count];
                }

                // 通知子进程处理数据
                for (int i = 0; i < n; i++)
                {
                    kill(pids[i], SIGUSR1);
                }

                // 等待所有子进程处理完毕
                for (int i = 0; i < n; i++)
                {
                    while (!received)
                    {
                        pause(); // 等待子进程发送 SIGUSR2
                    }
                    received = 0; // 重置信号接收标志
                }
            }
            break;
        }
        printf("Parent process %d\n", getpid());

        for (int i = 0; i < k; i++)
        {
            sum += data[i].data;
        }
        printf("parent sum = %d\n", sum);

        return 0;
    }
}
