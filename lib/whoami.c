#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall2(int, whoami, char*, name, int, size);

int main(int argc, char const *argv[])
{
    char name[23];
    int i = whoami(name, 23);
    printf("return value=%d\n", i);
    printf("%s\n", name);
    return 0;
}
