#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd1, fd2;
    char *src_addr, *dst_addr;
    struct stat statbuf;

    if (argc != 3)
    {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        exit(1);
    }

    if ((fd1 = open(argv[1], O_RDONLY)) == -1)
    {
        perror("open source");
        exit(1);
    }

    if (fstat(fd1, &statbuf) == -1)
    {
        perror("fstat");
        exit(1);
    }

    src_addr = mmap((char *)NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd1, (off_t)0);

    if (src_addr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    if ((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1)
    {
        perror("open destination");
        exit(1);
    }

    if (ftruncate(fd2, statbuf.st_size) == -1)
    {
        perror("ftruncate");
        exit(1);
    }
    dst_addr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    memcpy(dst_addr, src_addr, statbuf.st_size);

    munmap(src_addr, statbuf.st_size);
    munmap(dst_addr, statbuf.st_size);

    close(fd1);
    close(fd2);
    return 0;
}