#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main()
{
    char *current_dir = NULL;
    size_t size = 256; // 初始buffer大小

    while (1)
    {
        current_dir = (char *)malloc(size);
        // allocate 失敗
        if (current_dir == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        if (getcwd(current_dir, size) != NULL)
        {
            printf("%s\n", current_dir);
            free(current_dir);
            break;
        }
        else
        {
            // 如果buffer不夠大，getcwd會回傳NULL並設定errno為ERANGE
            if (errno == ERANGE)
            {
                free(current_dir);
                size *= 2; // 增加buffer大小
            }
            else
            {
                perror("getcwd");
                free(current_dir);
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
