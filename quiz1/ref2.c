#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define NUM_DATA 1000
#define NUM_CHILDREN 3 // 假设我们有k个子进程

typedef struct data
{
    int seg;
    int data;
} Data;

void process_data(Data *shared_data, int index)
{
    printf("Child %d processing data: seg=%d, data=%d\n", getpid(), shared_data[index].seg, shared_data[index].data);
}

int main()
{
    Data *shared_data;
    pid_t pids[NUM_CHILDREN];

    // Allocate a shared memory segment
    shared_data = mmap(NULL, sizeof(Data) * NUM_DATA, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize the shared memory with data
    for (int i = 0; i < NUM_DATA; i++)
    {
        shared_data[i].seg = i;
        shared_data[i].data = i * 2; // Just an example of initialization
    }

    // Fork children
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0)
        { // Child
            // Each child process can now access and process a portion of the shared data
            int segment_size = NUM_DATA / NUM_CHILDREN;
            int start_index = i * segment_size;
            int end_index = (i + 1) * segment_size;
            for (int j = start_index; j < end_index; j++)
            {
                process_data(shared_data, j);
            }
            // Optionally, synchronize with other processes or perform further tasks
            exit(EXIT_SUCCESS);
        }
    }

    // Parent waits for all child processes to complete
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    // Clean up the shared memory
    if (munmap(shared_data, sizeof(Data) * NUM_DATA) == -1)
    {
        perror("munmap");
    }

    return EXIT_SUCCESS;
}
