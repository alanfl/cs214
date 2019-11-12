#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv) {

    int fd = open("./test.c", O_RDONLY);

    if( fd==-1)
    {
         printf("Error: %s\n", strerror(errno));
         return -1;
    }

    char currentChar = '?';
    int numRead = -1;
 
    do
    {
        numRead = read(fd, &currentChar, 1);
        printf("%c", currentChar);
    } while (numRead > 0);
   
    return 0;
}
