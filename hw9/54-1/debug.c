#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define SEM_WRITE "/sem_write"
#define SEM_READ "/sem_read"

int main()
{
    sem_t *sem_write, *sem_read;
    sem_write = sem_open(SEM_WRITE, O_CREAT, 0644, 1);
    sem_read = sem_open(SEM_READ, O_CREAT, 0644, 0);
    sem_close(sem_write);
    sem_close(sem_read);
    sem_unlink(SEM_WRITE);
    sem_unlink(SEM_READ);
    return 0;
}