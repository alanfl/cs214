#include "library.h"
#include <stdio.h>

int main(int argc, char** argv) {
    char aString[] = {'h', 'i', '\0'};
    int intly[4] = {1, 2, 3};
    char * odd = (char*)intly;
    odd[12] = '\0';

    printf("%s\n", (char*) intly);

    int* ptr = (int*)malloc(sizeof(int) * 100);

    return 0;
}
