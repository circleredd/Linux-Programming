#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void inspectFD(const char *filePath)
{

    DIR *dir;
    if ((dir = opendir("/proc")) == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry, *fdEntry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == 4 && atoi(entry->d_name) != 0) // DT_DIR = 4 && isNumber
        {
            char fdPath[512];
            snprintf(fdPath, sizeof(fdPath), "/proc/%s/fd", entry->d_name);
            // printf("fd: %s\n", fdPath);
            DIR *fdDir = opendir(fdPath);
            if (fdDir == NULL)
            {
                perror("openfdDir");
                return;
            }

            // traverse all folders in fd directory of processes
            while ((fdEntry = readdir(fdDir)) != NULL)
            {
                // procDir->proc / 1 / fd
                char procDir[512];
                snprintf(procDir, sizeof(procDir), "/proc/%s/fd", entry->d_name);
                if (fdEntry->d_type == 10)
                {
                    char folderInFdPath[1024];
                    char targetPath[512];
                    snprintf(folderInFdPath, sizeof(folderInFdPath), "%s/%s", procDir, fdEntry->d_name);
                    // printf("proc: %s, folderName: %s\n", procDir, folderInFdPath);

                    // 使用readlink取得file descriptor的target path
                    ssize_t targetSize = readlink(folderInFdPath, targetPath, sizeof(targetPath) - 1);
                    if (targetSize != -1)
                    {
                        targetPath[targetSize] = '\0';
                        if (strcmp(targetPath, filePath) == 0)
                        {
                            // printf("File Descriptor %s points to: %s\n", entry->d_name, targetPath);
                            printf("PID: %s, FD: %s,  targetFile: %s\n", entry->d_name, fdEntry->d_name, targetPath);
                        }
                    }
                }
            }

            closedir(fdDir);
        }
    }

    return;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get absolute file path -> /home/username/Linux_Programming/hw4/4-3/.makefile.swp
    char *filePath = argv[1];

    inspectFD(filePath);

    return 0;
}