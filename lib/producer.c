#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall2(int, shmget, int, key, int, is_create);

_syscall1(int, shmat, int, shmid);

int main(int argc, char const *argv[])
{
    int shmid;
    int * a;
    int i;
    shmid = shmget(111, 1);
    printf("shmid=%d\n", shmid);
    a = (int *)shmat(shmid);
    printf("a=OX%p\n", a);
    for (i = 0 ; i < 100; i++) {
        *a = i;
        a++;
    }
    while(1) {
        pause();
    }
    return 0;
}
