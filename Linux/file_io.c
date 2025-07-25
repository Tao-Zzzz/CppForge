#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#define BUFF_SIZE 1024

int main(void)
{
    int fd;
    if ((fd = open("test.txt", O_RDWR | O_CREAT | O_APPEND)) < 0)
    {
        perror("文件打开失败");
        return -1;
    }

    close(fd);
}

void read_(int argc, char *argv[])
{
    // fd为文件描述符, n为读取字节数,total为总字节数
    int fd, n, total = 0;
    char buf[64];
    if (argc < 2)
    {
        printf("Usage: %s <file> \n", argv[0]);
        return -1;
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("open");
        return -1;
    }

    while ((n = read(fd, buf, 64)) > 0)
    {
        total += n;
    }

    printf("文件大小为: %d\n", total);
}

void write_(int argc, char *argv[])
{
    // 文件描述符
    int fd;
    // 每次从标准输入流读取的内容
    char buf[BUFF_SIZE];

    if (argc < 2)
    {
        printf("Usage %s <file> \n", argv[0]);
        return -1;
    }

    // 打开文件
    if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
    {
        perror("open");
        return -1;
    }

    while (fgets(buf, BUFF_SIZE, stdin) != NULL)
    {
        if (strcmp(buf, "quit\n") == 0)
        {
            break;
        }

        // 写入文件，写的长度取决于buf内容
        write(fd, buf, strlen(buf));
        // 清空buf内容
        memset(buf, 0, BUFF_SIZE);
    }

    close(fd);
    return 0;
}