#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    // 进程号
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        perror("fork");
        return -1;
    }
    else if (pid == 0)
    {
        // 获取自进程id
        printf("Child process : my pid is %d\n", getpid());
    }
    else
    {
        // 获取父进程id
        printf("Parent process: my pid is %d\n", getpid());
    }
    return 0;
}