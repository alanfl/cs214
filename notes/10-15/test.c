#include <stdio.h>

int main(int argc, char** argv) {

    int a = 4;
    int * ptr = &a;
    int ** test = &ptr;

    printf("test: %p\n", test);
    printf("*test: %p\n", *test);
    printf("**test: %d\n", **test);

    int b[3];
    printf("&(b[1]: %p\n", &(b[1]));
    printf("&(b+1): %p\n", &(b+2));

    return 1;
}
