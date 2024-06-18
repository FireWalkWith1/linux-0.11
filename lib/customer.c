#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall2(int, shmget, int, key, int, is_create);

_syscall1(int, shmat, int, shmid);

int main(int argc, char const *argv[])
{
    int shmid;
    int * a;
    shmid = shmget(111, 0);
    a = shmat(shmid, 0, 0);
    int i = 0;
    for (i = 0 ; i < 100; i++) {
        printf("%d\n", *a);
        a++;
    }
    /* code */
    return 0;
}
