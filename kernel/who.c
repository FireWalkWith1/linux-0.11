#include <segment.h>
#include <errno.h>

char *nameOuter;

int sys_iam(const char *name)
{
    char *nameInner;
    int len = 0;
    int finish = 0;
    while (len <= 23)
    {
        char c = get_fs_byte(name);
        nameInner[len] = c;
        if (c == '/0')
        {
            finish = 1;
            break;
        }
        name++;
        nameInner++;
    }
    if (!finish)
    {
        return -EINVAL;
    }
    nameOuter = nameInner;
    return len;
}

int sys_whoami(char *name, unsigned int size)
{
    int len = strlen(nameOuter);
    if (len == 0 || len > size)
    {
        return -EINVAL;
    }
    for (int i = 0; i <= len; i++)
    {
        put_fs_byte(nameOuter[len], name);
    }
    return len;
}