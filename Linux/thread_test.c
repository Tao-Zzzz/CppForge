#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern void *thread_func(void *arg);

int main(void)
{
    char message[32] = "Hello World";
    pthread_t work_thread;
    void *result;
    if (pthread_create(&work_thread, NULL, thread_func, message) != 0)
    {
        printf("fail to pthread_create");
        exit(-1);
    }

    // result接收了消息
    pthread_join(work_thread, &result);
    printf("result is %s\n", (char *)result);
    printf("message is %s\n", message);
    return 0;
}

void *thread_func(void *arg)
{
    char *msg = (char *)arg;
    memset(msg, 0, strlen(msg));
    strcpy(msg, "Hello IT HeiMa");
    pthread_exit(msg);
}