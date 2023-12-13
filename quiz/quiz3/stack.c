#include "stack.h"   /* Stack function definitions. */
#include <pthread.h> /* Posix threads. */
#include <stdio.h>
#include <unistd.h>

void push(data *globalStack, int oneInt)
{
    /***************************************************
    Push a character onto the stack. Return in the
    second argument the stack index corresponding to
    where the character is pushed.
    /**************************************************/

    char string[25];

    /* Get the lock before accessing the shared data in any way. */
    pthread_mutex_lock(&globalStack->sharedMutex);

    /* Test the data while under mutex protection, to
    see if the stack is pushable. Don’t change the
    data yet as the mutex lock is given up if
    pthread_cond_wait() is called; a free lock
    implies consistent data. */
    while (globalStack->index == STACK_SIZE)
    {

        pthread_cond_wait(&globalStack->sharedCond, &globalStack->sharedMutex);
    }

    /* Stack is pushable. Push the data. */
    globalStack->buffer[globalStack->index++] = oneInt;

    sprintf(string, "Pushed:\tInt: %d\tindex %d\n", oneInt, globalStack->index - 1);
    printf("%s\n", string);

    /* Notify waiting threads (poppers in this case) that the stack has changed and, due to the
    operation just completed, there is now something to pop. */
    pthread_cond_signal(&globalStack->sharedCond);

    /* Release the mutex. All done with shared data access. */
    pthread_mutex_unlock(&globalStack->sharedMutex);
}

/**************************************************/
int pop(data *globalStack)
{
    /***********************************************
    Pop a character from the stack. Return in the
    second argument the stack index corresponding to
    where the character is popped.
    /**************************************************/

    int toReturn;
    char string[25];

    /* Get the lock before accessing the shared data in any way. */
    pthread_mutex_lock(&globalStack->sharedMutex);

    /* Test the data while under mutex protection
    to see if the stack is pushable. Don’t change
    the data yet as the mutex lock is given up if
    pthread_cond_wait() is called; a free lock
    implies consistent data. */
    while (globalStack->index == 0)
    {
        pthread_cond_wait(&globalStack->sharedCond, &globalStack->sharedMutex);
    }

    /* Stack is poppable. Pop the data. */
    toReturn = globalStack->buffer[--globalStack->index];
    sprintf(string, "Pop:\tInt: %d\tindex %d\n", toReturn, globalStack->index);

    printf("%s\n", string);

    /* Notify waiting threads (pushers in this case)
    that the stack has changed and, due to the operation just completed, there is now something to push. */
    pthread_cond_signal(&globalStack->sharedCond);

    /* Release the mutex. All done with shared data access. */
    pthread_mutex_unlock(&globalStack->sharedMutex);

    return toReturn;
}