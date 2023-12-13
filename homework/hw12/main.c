#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <ftw.h>

#define MAX_EVENTS 1024
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (MAX_EVENTS * (EVENT_SIZE + 16))

int inotify_fd;

// callback fucntion，用於處理 nftw 每個文件的遍歷
int processFile(const char *filepath, const struct stat *info, int typeflag, struct FTW *pathinfo)
{
    int wd;

    if (typeflag == FTW_D)
    {
        // 在每個子目錄上啟動 inotify 監控
        wd = inotify_add_watch(inotify_fd, filepath, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);

        if (wd == -1)
        {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

int display_event()
{
    char buffer[BUF_LEN];
    ssize_t bytesRead = read(inotify_fd, buffer, BUF_LEN);
    if (bytesRead <= 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    for (char *ptr = buffer; ptr < buffer + bytesRead; ptr += sizeof(struct inotify_event) + ((struct inotify_event *)ptr)->len)
    {
        struct inotify_event *event = (struct inotify_event *)ptr;

        if (event->mask & IN_CREATE)
        {
            printf("File/directory created: %s\n", event->name);
        }
        else if (event->mask & IN_DELETE)
        {
            printf("File/directory deleted: %s\n", event->name);
        }
        else if (event->mask & IN_MOVED_FROM)
        {
            printf("File/directory moved from: %s\n", event->name);
        }
        else if (event->mask & IN_MOVED_TO)
        {
            printf("File/directory moved to: %s\n", event->name);
        }
    }
}

int main(int argc, char *argv[])
{

    // 初始化 inotify
    inotify_fd = inotify_init();
    if (inotify_fd == -1)
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // 使用 nftw 遍歷目錄並在每個子目錄上啟動 inotify 監控
    if (nftw(argv[1], processFile, 20, 0) == -1)
    {
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    printf("Monitoring directory: %s\n", argv[1]);

    while (1)
    {
        display_event();
    }

    close(inotify_fd);
    exit(EXIT_SUCCESS);
}
