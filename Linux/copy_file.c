#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define N 1024

int copy_by_object(void)
{
    FILE *src_file = fopen("src.mp4", "rb");
    if (src_file == NULL)
    {
        perror("无法打开文件: ");
        return -1;
    }

    FILE *dest_file = fopen("dest.mp4", "wb");
    if (dest_file == NULL)
    {
        perror("无法创建写入文件: ");
        // 关闭源文件
        fclose(src_file);
        return -1;
    }

    size_t element_num;
    char buffer[N];
    // 每次读取N字节写入目的文件
    while ((element_num = fread(buffer, 1, N, src_file)) > 0)
    {
        fwrite(buffer, 1, element_num, dest_file);
    }

    fclose(src_file);
    fclose(dest_file);
}

int copy_by_fileIO(int argc, char *argv[])
{
    // 源文件描述符，目的文件描述符，读取字节数
    int fds, fdt, n;
    char buf[N];
    if (argc < 3)
    {
        printf("Usage: %s <src_file> <dst_file> \n", argv[0]);
        return -1;
    }

    // 打开源文件
    if ((fds = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "open %s : %s \n", argv[1], strerror(errno));
        return -1;
    }

    // 打开目的文件
    if ((fdt = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "Open %s : %s \n", argv[2], strerror(errno));
        return -1;
    }

    // 读取源文件内容
    while ((n = read(fds, buf, N)) > 0)
    {
        // 向目的文件写
        write(fdt, buf, n);
    }
    // 关闭描述符
    close(fds);
    close(fdt);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "用法: %s <源文件>  <目标文件> ", argv[0]);
        return -1;
    }

    // 源文件名字
    const char *sourceFile = argv[1];
    // 目的文件名字
    const char *destFile = argv[2];
    // 打开源文件
    FILE *src = fopen(sourceFile, "rb");
    if (src == NULL)
    {
        perror("无法打开源文件");
        return -1;
    }

    // 打开目的文件
    FILE *dest = fopen(destFile, "wb");
    if (dest == NULL)
    {
        perror("无法创建目标文件");
        // 关闭源文件
        fclose(src);
    }

    // 循环读取文件内容
    char data[1024] = {0};
    while (fgets(data, 1024, src) != NULL)
    {
        fputs(data, dest);
        // 清空字节数组
        memset(data, 0, sizeof(data));
    }

    // 关闭文件
    fclose(src);
    fclose(dest);

    return 0;
}