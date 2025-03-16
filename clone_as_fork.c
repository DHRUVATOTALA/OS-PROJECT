#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define STACK_SIZE 1024*1024  // 1MB ka stack banayenge

int child_fn(void* arg) {
    usleep(rand() % 20000);  // Random delay to make order less predictable
    printf("Child process: PID= %d, PPID= %d\n", getpid(), getppid());
    fflush(stdout);          // Ensure output is immediately printed
    sched_yield();           // Yield CPU control
    return 0;
}

int main() {
    srand(time(NULL));  // Seed random number generator

    void *stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("malloc failed in allocating stack");
        exit(1);
    }

    pid_t pid = clone(child_fn, stack + STACK_SIZE, SIGCHLD, NULL);

    if (pid == -1) {
        perror("Clone toh fail ho gaya!");
        free(stack);
        exit(1);
    }

    usleep(rand() % 20000);  // Random delay for parent
    printf("Parent process: PID= %d, PPID= %d\n", getpid(), pid);
    fflush(stdout);

    usleep(50000);  // Allow time for child to complete
    free(stack);
    return 0;
}

