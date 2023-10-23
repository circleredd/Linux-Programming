#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <pwd.h>

int main()
{
    // 取得主機名稱
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        printf("hostname: %s\n", hostname);
    }
    else
    {
        perror("gethostname");
        return 1;
    }

    // 取得kernel版本
    struct utsname sys_info;
    if (uname(&sys_info) == 0)
    {
        printf("%s\n", sys_info.release);
    }
    else
    {
        perror("uname");
        return 1;
    }

    // 取得主機id
    uid_t hostid = gethostid();
    printf("hostid: %ld\n", (long)hostid);
    return 0;
}