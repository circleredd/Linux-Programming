#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pd[2];
    pid_t pid;

    if (pipe(pd) == -1)
    {
        fprintf(stderr, "fail to create pipe\n");
        exit(EXIT_FAILURE);
    }

    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    case 0:
        // child process
        close(pd[0]); // 關閉read端

        // 將stdout導向pipe的write端
        if (pd[1] != STDOUT_FILENO)
        {
            dup2(pd[1], STDOUT_FILENO);
            close(pd[1]);
        }
        execlp("ls", "ls", "-l", "/etc", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);

        break;

    default:
        // parent process
        close(pd[1]); // 關閉write端

        char buffer[1024];
        ssize_t bytes_read;

        // 從pipe的read端讀取資料
        while ((bytes_read = read(pd[0], buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pd[0]);
        break;
    }

    if (waitpid(pid, NULL, 0) == -1)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    return 0;
}