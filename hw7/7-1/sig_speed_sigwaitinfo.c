#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "tlpi_hdr.h"

static void handler(int sig)
{
    /* 可能的信号处理代码 */
}

#define TESTSIG SIGUSR1

int main(int argc, char *argv[])
{
    int numSigs, scnt, sig;
    pid_t childPid;
    sigset_t blockedMask;
    struct sigaction sa;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s num-sigs\n", argv[0]);
    }

    numSigs = getInt(argv[1], GN_GT_0, "num-sigs");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(TESTSIG, &sa, NULL) == -1)
    {
        errExit("sigaction");
    }

    /* 阻塞信號，直到準備好處理它們 */
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1)
    {
        errExit("sigprocmask");
    }

    switch (childPid = fork())
    {
    case -1:
        errExit("fork");

    case 0: /* child */
        for (scnt = 0; scnt < numSigs; scnt++)
        {
            if (kill(getppid(), TESTSIG) == -1)
            {
                errExit("kill");
            }
            sig = sigwaitinfo(&blockedMask, NULL);
            if (sig == -1)
            {
                errExit("sigwaitinfo");
            }
        }
        exit(EXIT_SUCCESS);

    default: /* parent */
        for (scnt = 0; scnt < numSigs; scnt++)
        {
            sig = sigwaitinfo(&blockedMask, NULL);
            if (sig == -1)
            {
                errExit("sigwaitinfo");
            }
            if (kill(childPid, TESTSIG) == -1)
            {
                errExit("kill");
            }
        }
        exit(EXIT_SUCCESS);
    }
}
