/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include "shell.h"
#include <unistd.h>
#include <string.h>

void run_command(char **myArgv)
{
    pid_t pid;
    int stat;
    /* Create a new child process.
     * Fill in code.
     */

    pid = fork();

    switch (pid)
    {

    /* Error. */
    case -1:
        perror("fork");
        exit(errno);

    /* Parent. */
    default:
        /* Wait for child to terminate.
         * Fill in code.
         */
        wait(&stat);

        /* Optional: display exit status.  (See wstat(5).)
         * Fill in code.
         */
        printf("[Parent] Child's exit status is [%d]\n", WEXITSTATUS(stat));

        return;

    /* Child. */
    case 0:
        /* Run command in child process.
         * Fill in code.
         */

        int count = 0;
        char command[128] = "";

        while (myArgv[count])
        {
            strcat(command, myArgv[count]);
            strcat(command, " ");
            count++;
        }

        printf("%s\n", command);
        system(command);
        /* Handle error return from exec */
        exit(errno);
    }
}
