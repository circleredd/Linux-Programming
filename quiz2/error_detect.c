#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

// 定義錯誤類型
#define TYPE1 SIGUSR1
#define TYPE2 SIGUSR2
#define TYPE3 SIGRTMIN

// p2 的signal handler
// signal handler1
void signalHandler_1(int sig)
{
    printf("P2 Recieve Error1\n");

    char cmd[1024];
    char to[] = "alanfang229@gmail.com";
    char subject[] = "type1 error";
    char body[] = "This is an type1 error.";

    snprintf(cmd, sizeof(cmd), "echo '%s' | mail -s '%s' %s", body, subject, to);

    system(cmd);
}

// signal handler2
void signalHandler_2(int sig)
{
    printf("P2 Recieve Error2\n");

    char cmd[1024];
    char to[] = "alanfang229@gmail.com";
    char subject[] = "type2 error";
    char body[] = "This is an type2 error.";

    snprintf(cmd, sizeof(cmd), "echo '%s' | mail -s '%s' %s", body, subject, to);

    system(cmd);
}

// signal handler3
void signalHandler_3(int sig)
{
    printf("P2 Recieve Error3\n");
    char cmd[1024];
    char to[] = "alanfang229@gmail.com";
    char subject[] = "type3 error";
    char body[] = "This is an type3 error.";

    snprintf(cmd, sizeof(cmd), "echo '%s' | mail -s '%s' %s", body, subject, to);

    system(cmd);
}

int main()
{

    pid_t pid;

    switch (pid = fork())
    {
    case -1:
        printf("fail\n");
        return 1;
    case 0:
        // p2 程序
        signal(TYPE1, signalHandler_1);
        signal(TYPE2, signalHandler_2);
        signal(TYPE3, signalHandler_3);
        while (1)
        {
            pause(); // 等待信號
        }
        break;
    default:
        // p1 程序
        srand(time(NULL));
        while (1)
        {
            int errType = rand() % 3; // 隨機選擇錯誤類型
            printf("Error type: %d\n", errType + 1);
            switch (errType)
            {
            case 0:
                kill(pid, TYPE1);
                break;
            case 1:
                kill(pid, TYPE2);
                break;
            case 2:
                kill(pid, TYPE3);
                break;
            }
            sleep(5);
        }
        break;
    }

    return 0;
}
