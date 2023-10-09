#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>

// 定義Process節點
struct ProcessNode
{
    int pid;
    int ppid;
    char command[256];
    struct ProcessNode *sibling;
    struct ProcessNode *child;
    struct ProcessNode *parent;
    struct ProcessNode *tmpLink;
};

// 創建樹節點
struct ProcessNode *createProcessNode(int pid, int ppid, const char *command)
{
    struct ProcessNode *node = (struct ProcessNode *)malloc(sizeof(struct ProcessNode));
    if (node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->pid = pid;
    node->ppid = ppid;
    strncpy(node->command, command, sizeof(node->command) - 1);
    node->sibling = NULL;
    node->child = NULL;
    node->tmpLink = NULL;
    return node;
}

// Traverse /proc目錄，建構樹
struct ProcessNode *buildProcessTree(struct ProcessNode *root)
{
    struct ProcessNode *last = NULL;
    DIR *dir = opendir("/proc");
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;

    // 找出/proc底下的所有process id
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == 4 && atoi(entry->d_name) != 0) // DT_DIR = 4 && isNumber
        {
            char statusPath[256];
            snprintf(statusPath, sizeof(statusPath), "/proc/%s/status", entry->d_name);

            // 打開所有process的status檔案
            FILE *statusFile = fopen(statusPath, "r");
            if (statusFile != NULL)
            {
                int pid = atoi(entry->d_name);
                char command[256] = "";
                char ppid[256] = "";
                char line[1024];
                int PPid;

                // 抓出Name和ppid
                while (fgets(line, sizeof(line), statusFile))
                {
                    if (sscanf(line, "Name: %s", command) == 1)
                    {
                        continue;
                    }
                    if (sscanf(line, "PPid: %s", ppid) == 1)
                    {
                        PPid = atoi(ppid);
                        break;
                    }
                }
                // 為每個Process建立一個Node, 並且將其用tmpLink暫時串在一起
                struct ProcessNode *node = createProcessNode(pid, PPid, command);

                if (root == NULL)
                {
                    root = node;
                    last = root;
                }
                else
                {
                    last->tmpLink = node;
                    last = last->tmpLink;
                }
                fclose(statusFile);
            }
        }
    }

    closedir(dir);

    // 將Node排序好
    sortNode(root);
    return root;
}

void sortNode(struct ProcessNode *root)
{
    struct ProcessNode *find, *cur;
    struct ProcessNode *newRoot = createProcessNode(0, -1, "test");
    newRoot->tmpLink = root;
    root = newRoot;

    // traverse all Node
    for (cur = root; cur != NULL; cur = cur->tmpLink)
    {
        int tmp_ppid = cur->ppid;

        // 尋找符合ppid的父節點並設定其parent和child指標，sibling則是指向同樣ppid的其他node
        for (find = root; find != NULL; find = find->tmpLink)
        {
            if (find->pid == tmp_ppid)
            {
                cur->parent = find;
                if (find->child == NULL)
                {
                    find->child = cur;
                }
                else
                {
                    struct ProcessNode *sibling = find->child;
                    while (sibling->sibling != NULL)
                    {
                        sibling = sibling->sibling;
                    }
                    sibling->sibling = cur;
                }
                break;
            }
        }
    }

    return;
}

// 印出ProcessTree
void printProcessTree(struct ProcessNode *node, int depth)
{
    struct ProcessNode *cur_layer = node;
    struct ProcessNode *sibs = NULL;
    struct ProcessNode *last_layer = cur_layer;
    int count = 0;
    while (cur_layer != NULL)
    {
        last_layer = cur_layer;
        printf("PID: %d, PPID: %d, Command: %s\n", cur_layer->pid, cur_layer->ppid, cur_layer->command);

        sibs = cur_layer->sibling;
        while (sibs)
        {
            for (int i = 0; i < count; i++)
            {
                printf("    ");
            }
            printf("PID: %d, PPID: %d, Command: %s\n", sibs->pid, sibs->ppid, sibs->command);

            if (sibs->sibling == NULL)
            {
                cur_layer = sibs->parent;
                if (cur_layer->sibling != NULL)
                {
                    cur_layer = cur_layer->sibling;
                    sibs = cur_layer->child;
                    continue;
                }
                break;
            }

            sibs = sibs->sibling;
        }
        count++;
        cur_layer = last_layer->child;
    }
}

int main()
{
    struct ProcessNode *root = NULL;
    root = buildProcessTree(root);

    if (root != NULL)
    {
        printf("Process Tree:\n");
        printProcessTree(root, 0);
    }
    return 0;
}