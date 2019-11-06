#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {

    int fd = open("test.txt", O_RDONLY);

    printf("%d %s\n", fd, strerror(errno));

    return 0;
}
