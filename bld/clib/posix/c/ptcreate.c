
#include "variety.h"
#include <pthread.h>
#include <process.h>
#include <malloc.h>
#include "rterrno.h"
#include "_ptint.h"

#include <stdio.h>

/* By default, allow OpenWatcom's thread library
 * to handle this...
 */
#define STACK_SIZE  0

struct __thread_pass {
    void      *(*start_routine)(void*); 
    void      *arg;
    pthread_t *thread;
    sem_t      registered;
};

void __thread_start( void *data )
{
struct __thread_pass *passed;
void *ret;

void *(*start_routine)(void*);
void *arg;

    passed = (struct __thread_pass *)data;

    printf("Register\n");
    passed->thread = __register_thread();
    
    printf("Register-done post\n");
    sem_post(&passed->registered);

    start_routine = passed->start_routine;
    arg = passed->arg;
    
    /* Destroy the passing structure */
    free(passed);

    ret = passed->start_routine(passed->arg);

    /* The pointer 'ret' must be returned to any waiting
     * "join" operations
     */
    pthread_exit(ret);
}

_WCRTLINK int pthread_create( pthread_t *thread, const pthread_attr_t *attr,
                              void *(*start_routine)(void*), void *arg )
{
    int ret;
    size_t stack_size;
    char *stack;
    struct __thread_pass *passed;
    
    if(thread == NULL) {
        _RWD_errno = EINVAL;
        return -1;
    }
    
    stack_size = STACK_SIZE;
    stack = NULL;
    
    if(attr != NULL) {
        if(attr->stack_size > STACK_SIZE)
            stack_size = attr->stack_size;
            
        if(attr->stack_addr != NULL)
            stack = (char *)attr->stack_addr;
    }
    
    passed = (struct __thread_pass *)malloc(sizeof(struct __thread_pass *));
    if(passed == NULL) {
        _RWD_errno = ENOMEM;
        return -1;
    }
    
    printf("Assign\n");
    passed->start_routine = start_routine;
    passed->arg = arg;
    passed->thread = NULL;
    
    printf("Sem init\n");
    sem_init(&passed->registered, 0, 1);
    
    if(stack == NULL && stack_size > 0) {
        stack = (char *)malloc(stack_size*sizeof(char *));
        if(stack == NULL) {
            free(passed);
            _RWD_errno = ENOMEM;
            return( -1 );
        }
    }
    
    printf("Sem wait registered\n");
    sem_wait(&passed->registered);
    
    printf("Launch\n");
    ret = _beginthread( __thread_start, stack, stack_size, (void *)passed );
    
    printf("Sem wait registered\n");
    sem_wait(&passed->registered);
    
    /* Wait for registration */
    sem_wait(&passed->registered);
    
    if(ret >= 0) {
        /* If the user provided a thread pointer, copy... */
        if(thread != NULL)
            memcpy(thread, passed->thread, sizeof(pthread_t));
        
        ret = 0;
    }
    
    /* Destroy the registration semaphore */
    sem_destroy(&passed->registered);

    return( ret );
}
