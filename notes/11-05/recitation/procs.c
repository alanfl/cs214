#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork
#include <sys/types.h> //macros
#include <sys/wait.h> //wait

int main(int argc, char** argv) {

    pid_t id;
    printf("Before fork: ID %d\n", getpid());

    id = fork(); // 1 process
    // 2 processes if successful
    
    if(id == 0) {
        printf("I am the child.\n");
        printf("I have PID: ID %d\n", getpid());
    } else if (id == -1) {
        printf("Fork failed.\n");
    } else {
        printf("I am the parent.\n");
        printf("Fork returned ID %d\n", id);
        printf("I have PID: ID %d\n", getpid());
    }

    return 1;
}

