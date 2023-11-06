#include <signal.h>
#include <stddef.h>
#define SIG_HOLD ((void (*)(int))2)

void (*sigset(int signo, void (*func)(int)))(int)
{
    struct sigaction act, oact;
    sigset_t set, oset;

    sigemptyset(&set);
    sigaddset(&set, signo);

    if (func == SIG_HOLD)
    {
        sigprocmask(SIG_BLOCK, &set, &oset);
    }
    else
    {
        act.sa_handler = func;
        act.sa_mask = set;
        act.sa_flags = 0;
        sigaction(signo, &act, &oact);
        sigprocmask(SIG_UNBLOCK, &set, &oset);
    }

    return oact.sa_handler;
}

int sighold(int signo)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, signo);
    return sigprocmask(SIG_BLOCK, &set, NULL);
}

int sigrelse(int signo)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, signo);
    return sigprocmask(SIG_UNBLOCK, &set, NULL);
}

int sigignore(int signo)
{
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    return sigaction(signo, &act, NULL);
}

int sigpause(int signo)
{
    sigset_t set, oset;
    sigemptyset(&set);
    sigaddset(&set, signo);
    sigprocmask(SIG_UNBLOCK, &set, &oset); // Unblock signo
    sigdelset(&oset, signo);               // Make sure signo is not blocked in oset
    return sigsuspend(&oset);              // Wait for any signal except signo
}

int main()
{
    return 0;
}