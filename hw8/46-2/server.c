#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_st
{
    long int msg_type;
    char text[BUFSIZ];
};

int main(int argc, char **argv)
{
    int msgid = -1;
    struct msg_st data;
    long int msgtype = 0; // 注意1
    int count = 0;

    // 建立message queue
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed width error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // 從mesage queue中取得訊息，直到遇到end訊息為止
    while (1)
    {
        if (msgrcv(msgid, (void *)&data, BUFSIZ, msgtype, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed width erro: %d", errno);
        }

        printf("Response: %d\n", count);

        // 遇到end结束
        if (strncmp(data.text, "end", 3) == 0)
        {
            break;
        }

        count += atoi(data.text);
    }

    // 删除mesage queue
    if (msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
    }

    exit(EXIT_SUCCESS);
}