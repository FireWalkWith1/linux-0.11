#include <linux/mm.h>
#include <linux/sched.h>
#include <errno.h>

struct shm
{
    int key;
    unsigned long page;
};

struct addr
{
    int pid;
    unsigned long address;
};

struct shm shms[10];

struct addr addrs[10];


int sys_shmget(int key, int is_create) {
    int i;
    if (is_create) {
        for (i = 0; i < 10; i++) {
            struct shm * shm = &shms[i];
            if (shm -> key == 0) {
                shm -> key = key;
                shm -> page = get_free_page();
                return i;
            }
        }
    }
    for (i = 0; i < 10; i++) {
            struct shm shm = shms[i];
            if (shm.key == key) {
                return i;
            }
        }
    
    return -EINVAL;
}


int sys_shmat(int shmid) {
    struct shm shm = shms[shmid];
    if (shm.page == 0) {
        return -EINVAL;
    }
    int i;
    for (i = 0; i < 10; i++) {
        struct addr * addr = &addrs[i];
        if (addr -> pid = current -> pid) {
            long address = addr -> address + 4096;
            long base = get_base(current -> ldt[1]);
            put_page(shm.page, base + address);
            addr -> address = address;
            return address;
        }

    }
    for (i = 0; i < 10; i++) {
        struct addr * addr = &addrs[i];
        if (addr -> pid = 0) {
            long address = current -> brk;
            long base = get_base(current -> ldt[1]);
            put_page(shm.page, base + address);
            addr -> pid = current -> pid;
            addr -> address = address;
            return address;
        }
    }
    return -EINVAL;

}