#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

_syscall2(int, whoami, char*, name, int, size);

int main(int argc, char const *argv[])
{
    char name[23];
    whoami(name, 23);
    printf("%s\n", name);
    printf("%s\n", "whoami");
    return 0;
}
