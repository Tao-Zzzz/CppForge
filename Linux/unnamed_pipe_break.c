#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char buf[1024];
    int pid, status, pfd[2];

    if (pipe(pfd) == EOF)
    {
        perror("pipe: ");
        return -1;
    }

    close(pfd[0]); // 关闭读端，同时也让fork后的子进程的读端是关闭的

    pid = fork();
    if (pid == 0)
    { // sub process.
        write(pfd[1], buf, 1024);
    }
    else
    {
        wait(&status); // 等待子进程结束，并用status获取其结束状态
    }
    printf("sub pricess status: %x\n", status);

    return 0;
}