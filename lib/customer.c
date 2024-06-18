#include <sys/shm.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int shmid;
    int * a;
    shmid = shmget(111, 4096, SHM_R|SHM_W);
    a = shmat(shmid, 0, 0);
    int i = 0;
    for (i = 0 ; i < 100; i++) {
        printf("%d\n", *a);
        a++;
    }
    /* code */
    return 0;
}
