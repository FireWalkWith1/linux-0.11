#include <asm/segment.h>
#include <errno.h>
#include <string.h>
#include <linux/kernel.h>

char nameOuter[24];

int sys_iam(const char *name)
{
    printk("sys_iam\n");
    char nameInner[24];
    int len = 0;
    int finish = 0;
    while (len <= 23)
    {
        char c = get_fs_byte(name);
        printk("c=%c\n", c);
        nameInner[len] = c;
        if (c == '\0')
        {
            finish = 1;
            break;
        }
        name++;
        len++;
    }
    printk("finish=%d\n", finish);
    if (!finish)
    {
        return -EINVAL;
    }
    printk("nameInner=%s\n", nameInner);
    // strcpy(nameOuter, nameInner);
    int i = 0;
    for(i = 0; nameInner[i] != '\0'; ++i)
    {
        nameOuter[i] = nameInner[i];
    }
    printk("nameOuter=%s\n", nameOuter);
    return len;
}

int sys_whoami(char *name, unsigned int size)
{
    int len = strlen(nameOuter);
    printk("len=%d\n", len);
    if (len == 0 || len > size)
    {
        return -EINVAL;
    }
    int i = 0;
    for (i = 0; i <= len; i++)
    {
        put_fs_byte(nameOuter[len], name);
    }
    return len;
}