/* Listing 48-2 svshm_xfr_writer
./svshm_xfr_writer < file & */

#include "semun.h" /* Definition of semun union */
#include "svshm_xfr.h"
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#define SHM_NAME "/my_shm"
#define SEM_WRITE "/sem_write"
#define SEM_READ "/sem_read"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void writer(int argc, char *argv[])
{
    int shmid, bytes, xfrs;
    struct shmseg *shmp;
    sem_t *sem_write, *sem_read;

    sem_write = sem_open(SEM_WRITE, O_CREAT, 0644, 1);
    sem_read = sem_open(SEM_READ, O_CREAT, 0644, 0);
    if (sem_write == SEM_FAILED || sem_read == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // open shared memory
    shmid = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmid == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(shmid, sizeof(struct shmseg));
    shmp = mmap(NULL, sizeof(struct shmseg), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
    if (shmp == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // 資料傳輸
    for (xfrs = 0, bytes = 0;; xfrs++, bytes += shmp->cnt)
    {
        sem_wait(sem_write);

        shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        sem_post(sem_read);

        if (shmp->cnt == 0) // 達到 EOF
            break;
    }

    // free resource
    sem_close(sem_write);
    sem_close(sem_read);
    sem_unlink(SEM_WRITE);
    sem_unlink(SEM_READ);

    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
