
#include <asm/system.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/segment.h>

struct queue {
    struct task_struct * task;
    struct queue * next;
};

typedef struct sem{
    char name[20];
    int value;
    struct queue *queue;
    int exist;
} semtable[20];

struct queue queues[1000];

int queueIndex = 0;

semtable sems;

typedef int sem_t;

void getName(const char *name, char *n);

int getIndex(const char* name);

int getFirstEmpty();

sem_t *sys_sem_open(const char *name, unsigned int value)
{
    cli();
    char n[20];
    getName(name, n);

    int index = getIndex(n);

    if (index == -1) {
        index = getFirstEmpty();
        if (index == -1) {
            sti();
            return -1; 
        }
        struct sem* sem = &sems[index];
        strcpy(sem->name, n);
        sem->value = value;
        sem->exist = 1;
    } else {
        struct sem* sem = &sems[index];
        sem->value = value;
    }
    printk("sys_sem_open,name=%s,value=%d,index=%d\n", n, value, index);
    sti();
    return (sem_t*)index;
}

int sys_sem_wait(sem_t *sem)
{
    cli();
    int index = (int)sem;
    if (index > 20 || sems[index].exist == 0) {
        sti();
        return -1; 
    }
    struct sem* s = &sems[index];
    s->value--;
    printk("sys_sem_wait,name=%s,value=%d,pid=%ld\n", s->name, s->value, current->pid);
    if (s->value < 0) {
        current -> state = 2;
        struct queue* qp = s->queue;
        if (qp == NULL) {
            struct queue* qu = &queues[queueIndex];
            queueIndex++;
            qu->task = current;
            qu->next = NULL;
            s->queue = qu;
        } else {
            struct queue* qu = &queues[queueIndex];
            queueIndex++;
            qu->task = current;
            qu->next = qp;
            s->queue = qu;
        }
        schedule();
    }
    sti();
    return 0;
}

int sys_sem_post(sem_t *sem)
{
    cli();
    int index = (int)sem;
    if (index > 20 || sems[index].exist == 0) {
        sti();
        return -1; 
    }
    struct sem* s = &sems[index];
    s->value++;
    printk("sys_sem_post,name=%s,value=%d,pid=%ld\n", s->name, s->value, current->pid);
    if (s->value <= 0) {
        struct queue* qp = s->queue;
        if (qp != NULL) {
            qp -> task -> state = 0;
            s->queue = qp -> next;
        }
    }
    sti();
    return 0;
}

int sys_sem_unlink(const char *name)
{
    cli();
    char n[20];
    getName(name, n);
    int index = getIndex(n);
    if (index == -1) {
        sti();
        return -1; 
    }
    (&(sems[index]))->exist = 0;
    sti();
    return 0;
}


void getName(const char *name, char *n)
{
    int finish = 0;
    int i;
    for (i = 0; i < 19; i++)
    {
        char c = get_fs_byte(name);
        n[i] = c;
        if (c == '\0')
        {
            finish = 1;
            break;
        }
        name++;
    }
    if(!finish) {
        n[19] = '\0';
    }
}

int getIndex(const char* name) {
    int i;
    for (i = 0; i < 20; i++) {
        struct sem sem = sems[i];
        if (sem.exist) {
            int index;
            for (index = 0; index < 20; index++) {
                if (name[index] != sem.name[index]) {
                    continue;
                }
                if (name[index] == '\0') {
                    return i;
                }
            }
        }
    }
    return -1;
}

int getFirstEmpty() {
    int i;
    for (i = 0; i < 20; i++) {
        struct sem sem = sems[i];
        if (!sem.exist) {
            return i;
        }
    }
}