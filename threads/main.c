#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void* thread_function(void *args)
{
    int loops = *((int *) args); //casting a void pointer into an integer - the dereference gives back whatever value is in the address
    int ix, ret;    //index and return
    for(ix = 0; ix < loops; ++ix)

    ret = pthread_mutex_lock(&mtx);
    if(ret != 0) return NULL;
    {
        ++glob;
    }
    return NULL;

    ret = pthread_mutex_unlock(&mtx);
    if(ret != 0) return NULL;
}

int main(int argc, char*argv[]) //argument count and argument vector - minimum 1 count
{                               //first argument in this argv vector is the name of the application                       
    pthread_t t1, t2;           //second thing that shows up is the arguments being passed through
    int loops, ret;
    loops = (argc > 1) ? atoi(argv[1]) : 1000; //starts converting the second element in the array into an integer

    ret = pthread_create(&t1, NULL, thread_function, &loops);
    ret = pthread_create(&t2, NULL, thread_function, &loops);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("total = %d\n", glob);

    return 0;
}

/*
There's no instruction that can increment a value in your memory - it's too slow.
When the interrupts happens makes this shit work or not
Atomic instruction is when something is a single instruction and hence can't be interrupted.
The magnitude of the error is equal to how many counters it can augment within the 2us time slice.
Homework for next week is to install Tailscale on the lap top and Raspberry Pi and leave Raspberry Pi at home.
++glob is not an atomic instruction so we get errors. 
Race condition means the result is non deterministic - you don't know what will happen. 
Race condition - when multiple processes will try to change a variable.
Critical section is the piece of code ++glob;
Race condition is when the multiple threads access a critical section (they try to change the variable)
Mutual exclusion will try to protect the problematic piece of code. 
Isolate it somehow - no more than one thread can acccess the piece of code THIS IS DONE BY LOCKING THAT SECTION.
A thread belongs to a process - a process can have multiple threads.
Global variables like flags cannot protect critical sections. 
A mutex is a flag that you can set and reset in one instruction (in an atomic instruction).
Set and reset are lock and unlock.
pthread_mutex_t the _t means its a new type.
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
Producer - consumer problem. *sounds familiar*

*/
