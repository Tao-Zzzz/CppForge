#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int fd;
    char buf[1024];

    fd = open("myfifo", O_WRONLY);
    if (fd == EOF)
    {
        perror("open w: ");
        return -1;
    }

    printf("pipe open success.\n");

    do
    {
        fgets(buf, 1024, stdin);
        if (strncmp(buf, "quit", 4) == 0)
            break;
        write(fd, buf, 1024);
    } while (strncmp(buf, "quit", 4) != 0);

    printf("you input the quit, exit.\n");

    return 0;
}