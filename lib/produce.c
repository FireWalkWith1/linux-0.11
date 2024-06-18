#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int shmid;
    int * a;
    shmid = shmget(111, 4096, 0600|IPC_CREAT);
    a = shmat(shmid, 0, 0);
    int i = 0;
    for (i = 0 ; i < 100; i++) {
        *a = i;
        a++;
    }
    return 0;
}
