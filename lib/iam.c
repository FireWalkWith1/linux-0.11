#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall1(int, iam, const char*, name);

int main(int argc, char const *argv[])
{
    int i = iam(argv[1]);
    // printf("return value=%d\n", i);
    return 0;
}
