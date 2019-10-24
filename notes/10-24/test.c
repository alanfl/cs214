#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void segfaultHandler(int num)
{
    printf("Segfaulted.\n");
    return;
}

void notifyExit()
{
    printf("DONE!\n");
}

int main(int argc, char** argv) {

    //signal(SIGSEGV, segfaultHandler);
    //int a = *((int*) 0);
    //++a;
    //printf("Value? %d\n", a);
   
    
    atexit(notifyExit);
    return 0;    
}
