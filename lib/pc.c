#define __LIBRARY__
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

typedef int sem_t;

_syscall2(sem_t*, sem_open, const char*, name, unsigned int, value);

_syscall1(int, sem_wait, sem_t*, sem);

_syscall1(int, sem_post, sem_t*, sem);

_syscall1(int, sem_unlink, const char*, name);

int fd;
sem_t *mutex_sem, *empty_sem, *full_sem;
const int buf_size = 10;
const int readNumPos = buf_size * sizeof(int);
void customer();
void produce();

int main(int argc, char * argv[])
{
	int i;
	int pid;
	pid_t pidt;
    fd = open("pc.txt", O_RDWR, 0770);
	if (!fd) {
		perror("打开文件失败\n");
		return -1;
	}
    mutex_sem = sem_open("/mutex", 1);
	if (mutex_sem == NULL) {
		perror("open mutex sem error");
		return -1;
	}
    empty_sem = sem_open("/empty", 10);
	if (empty_sem == NULL) {
		perror("open empty sem error");
		return -1;
	}
    full_sem = sem_open("/full", 0);
	if (full_sem == NULL) {
		perror("open full sem error");
		return -1;
	}
    for (i = 0; i < 10; i++) {
        pid = fork();
        if (pid == 0) {
            customer();
			exit(0);
        }
    }
    pid = fork();
	if (pid == 0) {
		produce();
		exit(0);
	}
	do {
		pidt = wait(NULL);
	} while (pidt > 0);
	
	sem_unlink("/mutex");
	sem_unlink("/empty");
	sem_unlink("/full");
	close(fd);
	return 0;
}

void produce() {
	int i, readPos;
	readPos = 0;
	lseek(fd, readNumPos, SEEK_SET);
	write(fd, &readPos, sizeof(int));
	
	for (i = 0; i < 500; i++) {
		sem_wait(empty_sem);
		sem_wait(mutex_sem);
		lseek(fd, (i % buf_size) * sizeof(int), SEEK_SET);
		const int buf = i;
		write(fd, &buf, sizeof(int));
		sem_post(mutex_sem);
		sem_post(full_sem);
	}
}

void customer() {
	int i;
	pid_t pid; 
	pid = getpid();
	for (i = 0; i < 500; i++) {
		sem_wait(full_sem);
		sem_wait(mutex_sem);
		int readPos;
		lseek(fd, readNumPos, SEEK_SET);
		read(fd, &readPos, sizeof(int));
		if (readPos == 500) {
			sem_post(mutex_sem);
			sem_post(full_sem);
			return;
		}
		int num;
		lseek(fd, (readPos % buf_size) * sizeof(int), SEEK_SET);
		read(fd, &num, sizeof(int));
		printf("%d:%d\n", pid, num);
		readPos++;
		lseek(fd, readNumPos, SEEK_SET);
		write(fd, &readPos, sizeof(int));
		if (readPos == 500) {
			sem_post(mutex_sem);
			sem_post(full_sem);
			return;
		}
		sem_post(mutex_sem);
		sem_post(empty_sem);
	}
}