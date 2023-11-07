
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

#define SHARED_MEMORY_SIZE 1024 // Size of the shared memory area
#define NUM_CHILDREN 5          // Number of child processes

// Signal handler for child processes
static void child_signal_handler(int sig)
{
    // The shared data pointer is set to be static and initialized once
    static char *shared_data = NULL;
    if (shared_data == NULL)
    {
        shared_data = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (shared_data == MAP_FAILED)
        {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }

    // Read data from the shared memory
    printf("Child %d received signal %d. Data: %s\n", getpid(), sig, shared_data);
}

int main()
{
    // Set up the shared memory area
    char *shared_data = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // Fork child processes
    pid_t pids[NUM_CHILDREN];
    for (int i = 0; i < NUM_CHILDREN; ++i)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        { // Child
            // Set up signal handler
            struct sigaction sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = child_signal_handler;
            sigaction(SIGUSR1, &sa, NULL);

            // Infinite loop, waiting for signals
            while (1)
            {
                pause(); // Wait for a signal
            }
            // Should never reach here
            exit(EXIT_SUCCESS);
        }
        else if (pids[i] < 0)
        {
            perror("fork");
            return EXIT_FAILURE;
        }
    }

    // Parent process
    // Write data to the shared memory
    const char *message = "Hello from parent!";
    strncpy(shared_data, message, SHARED_MEMORY_SIZE);

    // Send a signal to all child processes to read the shared data
    for (int i = 0; i < NUM_CHILDREN; ++i)
    {
        kill(pids[i], SIGUSR1);
    }

    // Wait for child processes to exit (if needed)
    for (int i = 0; i < NUM_CHILDREN; ++i)
    {
        waitpid(pids[i], NULL, 0);
    }

    // Clean up the shared memory
    if (munmap(shared_data, SHARED_MEMORY_SIZE) == -1)
    {
        perror("munmap");
    }

    return EXIT_SUCCESS;
}