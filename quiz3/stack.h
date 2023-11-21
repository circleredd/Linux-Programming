#define STACK_SIZE 10 /* Define a small stack size to cause contention. */

/* Define the data structure shared between the threads. */
static char buffer[STACK_SIZE]; /* Stack’s buffer */
static int index = 0;           /* Stack’s index. */

void push(char oneChar);
char pop();
