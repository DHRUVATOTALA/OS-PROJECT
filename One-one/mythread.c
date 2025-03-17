#include "mythread.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//Thread start ke liye wrapper function

static int thread_start(void *arg) {

	void **args = (void **)arg;
	/* 1. arg is initially a single void pointer.
	 * 2. Now, we assume that "arg" actually points to an array of two pointers:
	 * 	i. the function pointer (start_routine)
	 * 	ii. the actual argument for the function (real_arg)
	 * 3. Iska reason? we're going to get here when clone calls this function.
	 *    Since clone's 4th parameter is a (void*) pointer, the array of pointers is 
	 *    decayed into a pointer. We need to recast it so that it becomes a pointer to
	 *    an array of pointers! Read again if you didn't understand :) */

	void *(*start_routine)(void *) = (void *(*)(void *))args[0];
	void *real_arg = args[1];

	start_routine(real_arg);
	_exit(0);
}

//Naya thread banayenge using clone() 

int thread_create(mythread_t *thread, void *(*start_routine)(void *), void* arg) {
	thread->stack = malloc(STACK_SIZE);
	if(!thread->stack) {
		perror("malloc failed in allocating stack!");
		return -1;
	}

	//pass the function pointer and argument as a struct
	
	void* args[2] = {start_routine, arg};

	thread->pid = clone(thread_start, thread->stack + STACK_SIZE, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD, args);

	if(thread->pid == -1) {
		perror("clone failed!");
		return -1;
	}

	return 0;
}

//Join a thread (wait for its finishing)

int thread_join(mythread_t* thread) {
	if(waitpid(thread->pid, NULL, 0) == -1) {
		perror("waitpid fault!");
		return -1;
	}
	free(thread->stack);	//kaam ho gaya, free the allocated stack now
	return 0;
}

//exiting a thread
void thread_exit() {
	_exit(0);
}

//killing a thread using a signal
int thread_kill(mythread_t* thread, int sig) {
	return kill(thread->pid, sig);
}






