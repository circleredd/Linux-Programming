#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_queue"
#define MAX_SIZE BUFSIZ
#define MSG_STOP "end"

struct msg_st
{
    char text[MAX_SIZE];
};

int main(int argc, char **argv)
{
    mqd_t mq;
    struct msg_st data;
    int count = 0;
    struct mq_attr attr;

    // 設置消息隊列屬性
    // attr.mq_flags = 0;          // 阻塞flag
    // attr.mq_maxmsg = 10;        // queue中最大訊息數
    // attr.mq_msgsize = MAX_TEXT; // 每個訊息的最大大小
    // attr.mq_curmsgs = 0;        // 隊列中當前的訊息數（不需要設定）

    // 創建message queue
    // mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);// 以上述attr設定創建message queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, NULL);
    if (mq == (mqd_t)-1)
    {
        fprintf(stderr, "mq_open failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // 從message queue中讀取消息，直到遇到 "end" 為止
    while (1)
    {
        if (mq_receive(mq, (char *)&data, MAX_SIZE, NULL) == -1)
        {
            fprintf(stderr, "mq_receive failed with error: %d\n", errno);
            continue;
        }

        printf("Response: %d\n", count);

        // 遇到 "end" 時跳出迴圈
        if (strncmp(data.text, MSG_STOP, strlen(MSG_STOP)) == 0)
        {
            break;
        }

        count += atoi(data.text);
    }

    // 删除message queue
    if (mq_close(mq) == -1)
    {
        fprintf(stderr, "mq_close failed\n");
    }

    if (mq_unlink(QUEUE_NAME) == -1)
    {
        fprintf(stderr, "mq_unlink failed\n");
    }

    exit(EXIT_SUCCESS);
}
