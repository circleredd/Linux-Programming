#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
    int pd_in[2];
    int pd_out[2];
    char buf[1024];
    fgets(buf, sizeof(buf), stdin);

    if (pipe(pd_in) == -1)
    {
        perror("pipe error");
        return -1;
    }

    if (pipe(pd_out) == -1)
    {
        perror("pipe error");
        return -1;
    }

    switch (pid = fork())
    {
    case -1:
        perror("fork error");
        break;
    case 0:
        close(pd_in[1]);
        close(pd_out[0]);

        read(pd_in[0], buf, sizeof(buf));
        close(pd_in[0]);

        // convert to upper case
        int i = 0;
        for (i = 0; buf[i] != '\0'; i++)
        {
            if (buf[i] >= 'a' && buf[i] <= 'z')
            {
                buf[i] -= 32;
            }
        }

        write(pd_out[1], buf, i - 1);
        close(pd_out[1]);

        break;
    default:
        close(pd_in[0]);
        close(pd_out[1]);

        write(pd_in[1], buf, sizeof(buf));
        close(pd_in[1]);

        read(pd_out[0], buf, sizeof(buf));
        close(pd_out[0]);

        // write(STDOUT_FILENO, buf, sizeof(buf));
        printf("%s", buf);

        break;
    }

    return 0;
}