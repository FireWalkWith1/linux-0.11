#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
# include <stdio.h>

int main(int argc, char * argv[])
{
    printf("main...\n");
    int fd = open("pc.txt", O_RDWR, 0770);
    sem_t * mutex_sem = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t * empty_sem = sem_open("/empty", O_CREAT, 0666, 10);
    sem_t * full_sem = sem_open("/full", O_CREAT, 0666, 0);
    int i;
    int isFather = 1;
    for (i = 0; i < 10 && isFather; i++) {
        int wpid = fork();
        if (wpid == 0) {
            printf("fork...\n");
            isFather = 0;
            __pid_t pid = getpid();
            sem_wait(full_sem);
            sem_wait(mutex_sem);
            printf("read...\n");
            char buffer[1];
            read(fd, buffer, 1);
            printf("%d:%c\n", pid, buffer[0]);
            sem_post(mutex_sem);
            sem_post(empty_sem);
        }
    }
    if (isFather) {
        char c = 0;
        for (c = 0; c < 500; c++) {
            printf("wait empty_sem...\n");
            sem_wait(empty_sem);
            sem_wait(mutex_sem);
            printf("write...\n");
            write(fd, &c, 1);
            sem_post(mutex_sem);
            sem_post(full_sem);
        }
    }
    
	return 0;
}