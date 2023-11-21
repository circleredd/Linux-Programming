/****************************************************
This module exemplifies a threadsafe stack. It uses
a mutex to coordinate access to shared data. It
also demonstrates the use of condition variables by
which threads wait for the shared data to change,
and by which threads notify/resume each other when
the state has changed.
/***************************************************/

#include "stack.h" /* Stack function definitions. */
// #include "utils.h"   /* for printWithTime(). */
#include <pthread.h> /* Posix threads. */
#include <stdio.h>

/* Mutex lock for exclusive data access. */
// static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Condition var for coordinating threads. */
// static pthread_cond_t conditionVar = PTHREAD_COND_INITIALIZER;

/**************************************************/
void push(char oneChar)
{
    /***************************************************
    Push a character onto the stack. Return in the
    second argument the stack index corresponding to
    where the character is pushed.
    /**************************************************/

    char string[25];

    /* Get the lock before accessing the shared data in any way. */
    // pthread_mutex_lock(&mutex);

    /* Test the data while under mutex protection, to
    see if the stack is pushable. Don’t change the
    data yet as the mutex lock is given up if
    pthread_cond_wait() is called; a free lock
    implies consistent data. */
    while (index == STACK_SIZE)
    {
        printf("Stack is full\n");
        // printWithTime("push sleeping...\n");
        // pthread_cond_wait(&conditionVar, &mutex);
    }

    /* Stack is pushable. Push the data. */
    buffer[index++] = oneChar;
    sprintf(string, "Pushed:\tchar %c\tindex %d\n", oneChar, index - 1);
    printf("index: %d\n", index);
    printf("%c\n", oneChar);
    // printWithTime(string);

    /* Notify waiting threads (poppers in this case) that the stack has changed and, due to the
    operation just completed, there is now something to pop. */
    // pthread_cond_signal(&conditionVar);

    /* Release the mutex. All done with shared data access. */
    // pthread_mutex_unlock(&mutex);
    return;
}

/**************************************************/
char pop()
{
    /***********************************************
    Pop a character from the stack. Return in the
    second argument the stack index corresponding to
    where the character is popped.
    /**************************************************/

    char toReturn;
    char string[25];

    /* Get the lock before accessing the shared data in any way. */
    // pthread_mutex_lock(&mutex);

    /* Test the data while under mutex protection
    to see if the stack is pushable. Don’t change
    the data yet as the mutex lock is given up if
    pthread_cond_wait() is called; a free lock
    implies consistent data. */
    printf("index: %d\n", index);
    while (index == 0)
    {
        printf("Stack is empty\n");
        sleep(5);
        // printWithTime("pop sleeping...\n");
        // pthread_cond_wait(&conditionVar, &mutex);
    }

    /* Stack is poppable. Pop the data. */
    toReturn = buffer[--index];
    sprintf(string, "Pop:\tchar %c\tindex %d\n", toReturn, index);
    printf('%c\n', toReturn);
    // printWithTime(string);

    /* Notify waiting threads (pushers in this case)
    that the stack has changed and, due to the operation just completed, there is now something to push. */
    // pthread_cond_signal(&conditionVar);

    /* Release the mutex. All done with shared data access. */
    // pthread_mutex_unlock(&mutex);

    return toReturn;
}