#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
// userIdFromName
#include <pwd.h>
#include <grp.h>
#include <ctype.h>

uid_t /* Return UID corresponding to 'name', or -1 on error */
userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0') /* On NULL or empty string */
        return -1;                     /* return an error */

    u = strtol(name, &endptr, 10); /* As a convenience to caller */
    if (*endptr == '\0')           /* allow a numeric string */
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

// usage: ./userProcess username

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    uid_t userId = userIdFromName(argv[1]);

    // 打開"/proc"目錄
    DIR *dir;
    if ((dir = opendir("/proc")) == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;

    // 在 "/proc" 中將所有dtype == DT_DIR 且名稱為數字的路徑找過一輪
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == 4 && atoi(entry->d_name) != 0) // DT_DIR = 4 && isNumber
        {
            char statusPath[512];
            snprintf(statusPath, sizeof(statusPath), "/proc/%s/status", entry->d_name);

            // 打開/proc/pid/status檔案
            FILE *statusFile = fopen(statusPath, "r");
            if (statusFile != NULL)
            {
                uid_t uid = 0;
                char line[256];
                char command[256] = "";

                // 讀出command及Uid並進行比對
                while (fgets(line, sizeof(line), statusFile))
                {
                    if (sscanf(line, "Uid: %u", &uid) == 1)
                    {
                        if (uid == userId)
                        {
                            printf("Process ID: %s, Command Name: %s\n", entry->d_name, command);
                            break;
                        }
                    }
                    if (sscanf(line, "Name: %s", command) == 1)
                    {
                        // Remove trailing newline character
                        command[strlen(command) - 1] = '\0';
                    }
                }
                fclose(statusFile);
            }
        }
    }

    closedir(dir);

    return 0;
}