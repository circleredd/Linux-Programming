#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "errExit.h"

int
main(){
    int fd;
    fd = open("test.txt", O_RDWR);
    if(fd == -1)    
        // errexit("open");
        return -1;
    if(close(fd) == -1)
        // errexit("close");
        return -1;
    printf("Success\n");
    return 0;
    
}