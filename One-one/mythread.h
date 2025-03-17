#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>

#define STACK_SIZE 1024*1024	//Har thread ko 1MB diya jayega

typedef struct {
	int tid;	//Thread ID
	pid_t pid;	//Process ID of thread (since it is a kernel thread)
	void* stack;	//Stack allocated for thread
} mythread_t;

int thread_create(mythread_t* thread, void *(*start_routine)(void *), void* arg);
int thread_join(mythread_t* thread);
int thread_exit();
int thread_kill(mythread_t* thread, int sig);

#endif
