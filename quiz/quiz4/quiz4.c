#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#define SHM_NAME "/my_shm"
#define SEM_WRITE "/sem_write"
#define SEM_READ "/sem_read"
#define BUF_SIZE 1024

struct shmseg
{
    int cnt;            /* Number of bytes used in 'buf' */
    char buf[BUF_SIZE]; /* Data being transferred */
    sem_t *sem_write;
    sem_t *sem_read;
};
struct shmseg data;

void *reader()
{
    int shmid, bytes, xfrs;

    bytes = read(STDIN_FILENO, data.buf, BUF_SIZE);
    if (bytes == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    data.cnt = bytes;

    // Signal writer to start
    sem_post(data.sem_write);
    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);

    pthread_exit(NULL);
}

void *writer()
{
    sem_wait(data.sem_write);

    int shmid, xfrs, bytes;

    // Write data from data.buf to file
    bytes = write(STDOUT_FILENO, data.buf, data.cnt);
    if (bytes == -1)
    {
        perror("write");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    // Initialize semaphores
    data.sem_read = sem_open(SEM_READ, O_CREAT, 0666, 0);
    if (data.sem_read == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    data.sem_write = sem_open(SEM_WRITE, O_CREAT, 0666, 0);
    if (data.sem_write == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create reader and writer threads
    pthread_t reader_thread, writer_thread;
    pthread_create(&reader_thread, NULL, reader, NULL);
    pthread_create(&writer_thread, NULL, writer, NULL);

    // Wait for threads to finish
    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);

    // Close semaphores
    sem_close(data.sem_write);
    sem_close(data.sem_read);

    // Unlink semaphores
    sem_unlink(SEM_WRITE);
    sem_unlink(SEM_READ);

    // printf("Data transfer completed.\n");

    return 0;
}