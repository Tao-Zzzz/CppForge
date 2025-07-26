#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
    if (mkfifo("myfifo", 0666) == EOF)
    {
        perror("mkfifo: ");
        return -1;
    }

    return 0;
}