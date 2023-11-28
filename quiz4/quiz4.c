#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#include "semun.h" /* Definition of semun union */
#include "svshm_xfr.h"

#define SHM_NAME "/my_shm"
#define SEM_WRITE "/sem_write"
#define SEM_READ "/sem_read"

struct shmseg *data;

void *writer(int argc, char *argv[])
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

void *reader(int argc, char *argv[])
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

main(int argc, char *argv[])
{
    printf("This is quiz4\n");
    // create two threads, one for reader, one for writer
    pthread_t tid[2];
    pthread_create(&tid[0], NULL, reader, NULL);
    pthread_create(&tid[1], NULL, writer, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}