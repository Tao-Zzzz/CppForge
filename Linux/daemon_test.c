#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    pid_t pid;
    FILE *fp;
    time_t t;
    int i;
    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(-1);
        return 0;
    }

    if (pid > 0)
    {
        exit(0);
        return 0;
    }

    // 一套丝滑小连招
    setsid();
    umask(0);
    chdir("/tmp");
    for (i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }

    if ((fp = fopen("time.log", "a")) == NULL)
    {
        perror("fopen");
        exit(-1);
        return 0;
    }

    // 一直写入
    while (1)
    {
        time(&t);
        fprintf(fp, "%s", ctime(&t));
        fflush(fp);
        sleep(1);
    }
    return 0;
}