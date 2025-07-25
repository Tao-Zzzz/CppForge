#include <stdio.h>

int input(void)
{
    FILE *fp = fopen("outfile", "w");
    if (fp == NULL)
    {
        perror("创建文件失败: ");
        return -1;
    }
    // 学生结构体类型
    struct Student
    {
        int no;
        char name[10];
        float score;
    };

    struct Student s[] = {{1, "haha", 99}, {2, "lala", 99}};
    fwrite(s, sizeof(struct Student), 2, fp);
    fclose(fp);
    return 0;
}

int main(void)
{
    int element_num;
    FILE *fp = fopen("outfile", "r");
    if (fp == NULL)
    {
        perror("打开文件失败:");
        return -1;
    }

    struct Student
    {
        int no;
        char name[10];
        float score;
    };

    struct Student s[5];
    while ((element_num = fread(s, sizeof(struct Student), 5, fp)) > 0)
    {
        for (int i = 0; i < element_num; i++)
        {
            printf("%d\t%s\t%f\n", s[i].no, s[i].name, s[i].score);
        }
    }

    fclose(fp);
    return 0;
}