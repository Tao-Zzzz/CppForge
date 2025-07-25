#include <stdio.h>
// time()、localtime()
#include <time.h>
#include <string.h>
#define PATH "test4.txt"
// sleep(秒)
#include <unistd.h>

int main(void)
{
    FILE *fp = fopen(PATH, "a+");
    if (fp == NULL)
    {
        perror("fopen: ");
        return -1;
    }

    // 行数量
    int line_num = 0;
    // 读取文件的字符串缓冲区
    char line[1024];
    // 时间对象
    time_t t;
    // struct tm结构存储时间数据
    struct tm tm_result;
    // tm结构体指针，调用localtime函数后指向上面的tm_result变量
    struct tm *tp;

    while (fgets(line, 1024, fp) != NULL)
    {
        // 如果一行数据比1024少，就需要判断末尾字符
        if (line[strlen(line) - 1] == '\n')
        {
            line_num++;
        }
    }

    // 写入
    while (1)
    {
        time(&t);
        tp = localtime_r(&t, &tm_result);
        if (tp == NULL)
        {
            fprintf(stderr, "无法获取localtime.\n");
            return -1;
        }
        fprintf(fp, "%d, %d-%02d-%02d %02d:%02d:%02d\n", ++line_num, tp->tm_year + 1900,
                tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
        // 默认文件是全缓冲，需要强制刷新
        fflush(fp);
        // 每隔1s刷新
        sleep(1);
    }

    fclose(fp);
    return 0;
}