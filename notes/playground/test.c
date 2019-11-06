#include <stdio.h>
#include <stdlib.h>

int testFunction(int x, char c) {

    printf("Hello there.\n");
}

int main(int argc, char** argv) {

    long int test = &testFunction;
    int num = test(4, 'a');

    return 0;
}
