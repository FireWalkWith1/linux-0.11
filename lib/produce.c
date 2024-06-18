#define __LIBRARY__
#include <unistd.h>

_syscall2(int, shmget, int, key, int, is_create);

_syscall1(int, shmat, int, shmid);

int main(int argc, char const *argv[])
{
    int shmid;
    int * a;
    shmid = shmget(111, 1);
    a = shmat(shmid);
    int i = 0;
    for (i = 0 ; i < 100; i++) {
        *a = i;
        a++;
    }
    return 0;
}
