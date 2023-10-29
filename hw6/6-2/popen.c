#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

FILE *custom_popen(const char *command, const char *mode)
{
    pid_t pid;
    int pd[2];
    FILE *fp;

    if (pipe(pd) == -1)
    {
        perror("pipe error");
        return NULL;
    }

    switch ((pid = fork()))
    {
    case -1:
        perror("fork error");
        return NULL;
    case 0: // Child process
        close(pd[0]);
        if (strcmp(mode, "r") == 0)
        {
            dup2(pd[1], STDOUT_FILENO);
            close(pd[1]);
            execl("/bin/sh", "sh", "-c", command, (char *)0);
        }
        else if (strcmp(mode, "w") == 0)
        {
            dup2(pd[0], STDIN_FILENO);
            close(pd[0]);
            execl("/bin/sh", "sh", "-c", command, (char *)0);
        }
        else
        {
            perror("Invalid mode");
            exit(1);
        }
        exit(1);
    default: // Parent process
        close(pd[1]);
        if (strcmp(mode, "r") == 0)
        {
            if ((fp = fdopen(pd[0], "r")) == NULL)
            {
                perror("fdopen error");
                close(pd[0]);
                return NULL;
            }
        }
        else if (strcmp(mode, "w") == 0)
        {
            if ((fp = fdopen(pd[1], "w")) == NULL)
            {
                perror("fdopen error");
                close(pd[1]);
                return NULL;
            }
        }
        else
        {
            perror("Invalid mode");
            return NULL;
        }
        break;
    }

    return fp;
}

int custom_pclose(FILE *fp)
{
    int status;
    pid_t pid;

    int fd = fileno(fp);
    if (fclose(fp) == EOF)
    {
        perror("fclose error");
        return -1;
    }

    if ((pid = waitpid(-1, &status, 0)) == -1)
    {
        perror("waitpid error");
        return -1;
    }

    return status;
}

int main(int argc, char *argv[])
{
    char command[32];
    strcpy(command, argv[1]);
    for (int i = 2; i < argc; i++)
    {
        strcat(command, " ");
        strcat(command, argv[i]);
    }

    FILE *fp = custom_popen(command, "r");
    if (fp == NULL)
    {
        perror("custom_popen error");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    int status = custom_pclose(fp);
    if (status == -1)
    {
        perror("custom_pclose error");
        return 1;
    }

    return 0;
}
