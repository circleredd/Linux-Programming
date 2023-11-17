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
#define BUF_SIZE 1024 // 假設的buffer size

struct shmseg
{
    int cnt;
    char buf[BUF_SIZE];
};

int main(int argc, char *argv[])
{
    int shmid, xfrs, bytes;
    struct shmseg *shmp;
    sem_t *sem_write, *sem_read;

    // open semaphore
    sem_write = sem_open(SEM_WRITE, 0);
    sem_read = sem_open(SEM_READ, 0);
    if (sem_write == SEM_FAILED || sem_read == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // open shared memory
    shmid = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shmid == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ, MAP_SHARED, shmid, 0);
    if (shmp == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // 從shared memory讀取資料，並輸出到stdout
    for (xfrs = 0, bytes = 0;; xfrs++)
    {
        sem_wait(sem_read);

        if (shmp->cnt == 0) // 寫入端已經結束(遇到EOF)
            break;
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        sem_post(sem_write);
    }

    // free
    munmap(shmp, sizeof(struct shmseg));
    close(shmid);
    sem_close(sem_write);
    sem_close(sem_read);

    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
