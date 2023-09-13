#include "errExit.h"

int errexit(char *format, ...)
{
        va_list args;
        char buf[1024];

        va_start(args, format);
        //vfprintf(stderr, format, args);
        vsprintf(buf, format, args);
        va_end(args);
        fprintf(stderr, "ERROR: %s\n", buf);
        exit(1);
}