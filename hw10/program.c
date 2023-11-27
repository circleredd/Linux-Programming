#include <stdio.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct Data
{
    int seq;
    char message[80];
} Data;

Data *addr;
int *shared_count;

void generator(Data *data, int data_num)
{
    for (int i = 0; i < data_num; i++)
    {
        data[i].seq = i;
        sprintf(data[i].message, "This is message %d", i);
    }
    return;
}



void sig_handler_exit(int sig)
{

    if (sig == SIGTERM)
    {
        printf("EXIT PID=%d\n", getpid());
        exit(0);
    }
}

void sig_handler_data(int sig)
{

    if (sig == SIGUSR1)
    {
        int i = 0;
        int num_batch = shared_count[1];
        int count = 0;
        for (i = 0; i < num_batch; i++)
        {
            printf("%d -> Data: %s\n", getpid(), addr[i].message);
            count++;
        }
        shared_count[0] += count;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: ./program <DataNum(M)> <Rate(R)> <ProcessNum(N)> <bufferSize(B)>\n");
        exit(1);
    }
    int M = atoi(argv[1]), R = atoi(argv[2]), N = atoi(argv[3]), B = atoi(argv[4]);
    printf("M: %d, R: %d, N: %d, B: %d\n", M, R, N, B);
    Data data[M];
    int fd, fd2;
    int *count;

    pid_t pids[N];

    generator(data, M);

    if ((fd = open("mapfile", O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
    {
        perror("open");
        exit(1);
    }

    if (ftruncate(fd, sizeof(Data) * B) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    if ((fd2 = open("mapfile2", O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
    {
        perror("open");
        exit(1);
    }

    if (ftruncate(fd2, sizeof(int)) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    addr = mmap(NULL, sizeof(Data) * B, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    shared_count = mmap(NULL, sizeof(int) * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    shared_count[0] = 0; // 成功重送資料總數
    shared_count[1] = B; // 每次送資料batch數量

    if (addr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    if (shared_count == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(fd);
    close(fd2);

    signal(SIGTERM, sig_handler_exit);
    signal(SIGUSR1, sig_handler_data);

    for (int i = 0; i < N; i++)
    {
        switch (pids[i] = fork())
        {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            printf("Child %d is created\n", getpid());
            while (1)
            {
                pause();
            }

        default:
            break;
        }
    }
    int num_of_data = 0;

    while (num_of_data < M)
    {

        int batch_size = (M - num_of_data > B) ? B : (M - num_of_data);
        if (batch_size < B)
        {
            shared_count[1] = batch_size;
        }

        for (int j = 0; j < batch_size; j++)
        {
            strcpy(addr[j].message, data[num_of_data + j].message);
        }

        num_of_data += batch_size;

        for (pid_t i = 0; i < N; i++)
        {
            kill(pids[i], SIGUSR1);                        
        }
        usleep(10*R);
        
    }

    int status;
    for (pid_t i = 0; i < N; i++)
    {
        // send a signal to child to end themslevs
        kill(pids[i], SIGTERM);
        waitpid(pids[i], &status, 0);
        printf("Child %d is terminated\n", pids[i]);
    }
    


    int totalMessage = M * N;
    int receivedMessage = shared_count[0];
    float lossRate = 1 - ((float)receivedMessage / totalMessage);
    printf("Total messages: %d\n", totalMessage);
    printf("Sum of received messages by all consumers: %d\n", receivedMessage);
    printf("Loss rate: %.2f%%\n", lossRate * 100);

    // free
    munmap(addr, sizeof(Data) * B);
    munmap(shared_count, sizeof(int));
    exit(0);
    // return 0;
}
