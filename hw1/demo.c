#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "errExit.h"

int
main(int argc, char *argv[]){
    int fd;
    ssize_t numWritten;
    char *buf = &argv[2][0];
    fd = open(argv[1], O_RDWR);
    if(fd == -1)    
        // errexit("open");
        return -1;
    numWritten = write(fd, buf, 4);
    if(numWritten == -1)
	    return -1;
    if(close(fd) == -1)
        // errexit("close");
        return -1;
    printf("Success\n");
    printf("%s %s\n", argv[0], argv[1]);
    return 0;
    
}
