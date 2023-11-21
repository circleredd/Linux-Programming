#include <pthread.h>

#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 3 /* Define a small stack size to cause contention. */

/* Define the data structure shared between the threads. */
static int buffer[STACK_SIZE]; /* Stack’s buffer */
static int index = 0;          /* Stack’s index. */

typedef struct
{
    int index;
    int buffer[STACK_SIZE];
    pthread_mutex_t sharedMutex;
    pthread_cond_t sharedCond;
} data;

void push(data *globalStack, int oneInt);
int pop(data *globalStack);

#endif