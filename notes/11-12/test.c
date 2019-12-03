#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv) {

	int fd = open("./newfile.c", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	lseek(fd, 1024, SEEK_END);

	char end = '\0';

	write(fd, &end, 1);


	close(fd);

	return 0;
}
