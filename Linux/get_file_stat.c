#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // 存储文件信息
    struct stat buf;
    int n;
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    // 获取属性
    if (lstat(argv[1], &buf) < 0)
    {
        perror("lstat:");
        return -1;
    }

    // 给大家打印看看S_IFMT是0170000
    printf("S_IFMT: %o\n", S_IFMT);

    switch (buf.st_mode & S_IFMT)
    {
    // 是文件
    case S_IFREG:
        printf("-");
        break;
    // 是目录
    case S_IFDIR:
        printf("d");
        break;
    }

    // 判断每一位对应的权限
    for (n = 8; n >= 0; n--)
    {
        // 模式和对应的位按位&获取对应的权限
        if (buf.st_mode & (1 << n))
        {
            switch (n % 3)
            {
            case 2:
                printf("r");
                break;
            case 1:
                printf("w");
                break;
            case 0:
                printf("x");
                break;
            }
        }
        else
        {
            printf("-");
        }
    }

    // 打印文件大小
    printf(" %lu", buf.st_size);

    // 定义获取本地时间的指针
    struct tm *tp;
    tp = localtime(&buf.st_mtime);
    printf(" %d-%02d-%02d", tp->tm_year + 1900,
           tp->tm_mon + 1, tp->tm_mday);
    printf(" %s\n", argv[1]);
}