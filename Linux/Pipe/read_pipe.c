#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
    int fd;
    char buf[1024];
    fd = open("myfifo", O_RDONLY);
    if (fd == EOF)
    {
        perror("open r: ");
        return -1;
    }

    printf("pipe open success.\n");

    while (read(fd, buf, 1024) > 0)
    {
        printf("you enter message length is: %lu(without \\n)\n", strlen(buf) - 1);
    }

    printf("read function get result 0, exit.\n");
    return 0;
}