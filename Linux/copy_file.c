#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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