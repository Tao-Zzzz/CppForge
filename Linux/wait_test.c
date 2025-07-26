#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status = 0;
    if ((pid = fork()) < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        sleep(1);
        exit(2);
        return 0;
    }
    else
    {
        // 父进程
        waitpid(pid, &status, 0);
        printf("子进程返回值： %d\n", WEXITSTATUS(status));
        printf("子进程是否为正常结束： %d\n", WIFEXITED(status));
        exit(0);
        return 0;
    }
}