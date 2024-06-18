#define __LIBRARY__
#include <unistd.h>

_syscall2(int, shmget, int, key, int, is_create);

_syscall1(int, shmat, int, shmid);

int main(int argc, char const *argv[])
{
    int shmid;
    int * a;
    int i;
    shmid = shmget(111, 1);
    printf("shmid=%d", shmid);
    a = (int *)shmat(shmid);
    for (i = 0 ; i < 100; i++) {
        *a = i;
        a++;
    }
    return 0;
}
