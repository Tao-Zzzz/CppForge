#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define N 1024

int main(void)
{
    int shmid;
    key_t key;
    char *buf; // shm addr pointer.

    // create key
    key = ftok("/tmp", 's');
    if (key == EOF)
    {
        perror("ftok: ");
        return -1;
    }

    // create shm
    shmid = shmget(key, N, IPC_CREAT | 0666);
    if (shmid == EOF)
    {
        perror("shmget: ");
        return -1;
    }

    printf("pid: %d\tkey: %x\tshmid: %d\n", getpid(), key, shmid);

    // mapping shm
    buf = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (buf == (char *)-1)
    {
        perror("shmat: ");
        return -1;
    }

    while (1)
    {
        if (strlen(buf) > 0)
        {
            printf("content: %s", buf);
            if (strncmp(buf, "quit", 4) == 0)
                break;
        }
        else
        {
            printf("shared memory is empty, sleep 1s. \n");
        }
        sleep(1);
    }

    // unmapping shm
    shmdt(buf);

    // rm shm
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}