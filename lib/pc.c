#include <fcntl.h>

int main(int argc, char * argv[])
{

    int fd = open("pc.txt", O_RDWR, 0770);
    
    int i;
    for (i = 0; i <10; i++) {
        int wpid1 = fork();
        if (wpid1 == 0) {

        }
    }
    
	
	return 0;
}