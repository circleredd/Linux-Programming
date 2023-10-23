#include <stdio.h>
#include <time.h>

int main()
{
    time_t current_time;
    struct tm *time_info;
    char time_str[80];

    time(&current_time);
    time_info = localtime(&current_time);

    if (time_info == NULL)
    {
        perror("localtime");
        return 1;
    }

    strftime(time_str, sizeof(time_str), "%b %e (%a), %Y %I:%M %p", time_info);
    printf("%s\n", time_str);

    return 0;
}
