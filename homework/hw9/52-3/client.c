#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MAX_TEXT 512
#define QUEUE_NAME "/my_queue"

int main(int argc, char *argv[])
{
    mqd_t mq;
    char msgcontent[MAX_TEXT];
    struct mq_attr attr;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(msgcontent, argv[1], MAX_TEXT);
    msgcontent[MAX_TEXT - 1] = '\0'; // 確保字串以 null 結尾

    // 設置消息隊列屬性
    // attr.mq_flags = 0;          // 阻塞flag
    // attr.mq_maxmsg = 10;        // queue中最大訊息數
    // attr.mq_msgsize = MAX_TEXT; // 每個訊息的最大大小
    // attr.mq_curmsgs = 0;        // 隊列中當前的訊息數（不需要設定）

    // 創建message queue
    // mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr); // 以上述attr設定創建message queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, NULL);
    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // 發送訊息
    if (mq_send(mq, msgcontent, strlen(msgcontent) + 1, 0) == -1)
    {
        perror("mq_send");
        mq_close(mq);
        exit(EXIT_FAILURE);
    }

    // 關閉message queue
    if (mq_close(mq) == -1)
    {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    return 0;
}
