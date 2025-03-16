#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>	//for SYS_gettid

#define STACK_SIZE 1024*1024	//1MB ka stack banayenge

int child_fn(void* arg) {
	printf("Thread-Like Child: PID=%d, TID=%ld\n",getpid(),syscall(SYS_gettid));
	printf("Child sees shared variable: %d\n", (*(int*)arg));
	(*(int*)arg)++;	//shared variable ko modify karenge
	return 0;
}

int main() {
	void* stack = malloc(STACK_SIZE);
	if(!stack) {
		perror("malloc fail kar gaya");
		exit(1);
	}
	
	int shared_variable = 10;	//shared memory
	
	pid_t tid = clone(child_fn, stack + STACK_SIZE, CLONE_VM | CLONE_SIGHAND | CLONE_THREAD | CLONE_FILES, &shared_variable);

	if(tid == -1) {
		perror("clone fail kar gaya");
		free(stack);
		exit(1);
	}

	sleep(1);	//let the child execute
	printf("Thread-Like Parent: PID=%d, TID=%ld\n",getpid(),syscall(SYS_gettid));
	printf("Parent sees shared variable: %d\n", shared_variable);

	free(stack);
	return 0;
}

