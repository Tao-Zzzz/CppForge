#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int sig)
{
    if (sig == SIGCONT)
    {
        printf("receive SIGCONT signal\n");
    }
    if (sig == SIGALRM)
    {
        printf("receive SIGALRM signal\n");
    }
}

int main(void)
{
    signal(SIGCONT, sig_handler);
    signal(SIGALRM, sig_handler);
    printf("pid: %d\n", getpid());
    alarm(10);
    pause();
    printf("I am week up.\n");
    return 0;
}