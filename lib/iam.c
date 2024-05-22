#define __LIBRARY__
#include <unistd.h>

_syscall1(int, iam, char*, name);

int main(int argc, char const *argv[])
{
    iam(argv[1]);
    return 0;
}
