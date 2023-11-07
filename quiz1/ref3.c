/* ...[省略其他代码部分]... */

addr = mmap(NULL, sizeof(data) * data_num, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
if (addr == MAP_FAILED)
{
    perror("mmap");
    exit(1);
}

close(fd); // 关闭文件描述符，因为它不再需要

// 使用正确的转换将addr转换为data类型的指针
data *shared_data = (data *)addr;

for (int i = 0; i < data_num; i++)
{
    shared_data[i].seg = data[i].seg;
    shared_data[i].data = data[i].data;
}

// create n processes
for (int i = 0; i < n; i++)
{
    switch (pids[i] = fork())
    {
    case -1:
        perror("fork");
        exit(1);
    case 0:
        // child process
        pid_t pid = getpid();
        int local_sum = 0; // 每个子进程都应该有自己的sum变量

        // 子进程从共享内存中读取数据
        for (int j = 0; j < data_num; j++)
        {
            local_sum += shared_data[j].data;
        }

        printf("Child process %d, sum = %d\n", pid, local_sum);
        munmap(shared_data, sizeof(data) * data_num); // 子进程使用完毕后，取消映射
        exit(0);
    default:
        // parent process
        break;
    }
}

for (int i = 0; i < n; i++)
{
    wait(NULL); // 等待所有子进程结束
}

// 父进程处理完毕后，取消映射并清理共享内存
munmap(shared_data, sizeof(data) * data_num);

/* ...[省略其他代码部分]... */