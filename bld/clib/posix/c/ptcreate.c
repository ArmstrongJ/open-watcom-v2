
#include "variety.h"
#include <pthread.h>
#include <process.h>
#include <malloc.h>
#include "rterrno.h"
#include "_ptint.h"

#include <string.h>

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

    passed->thread = __register_thread();

    sem_post(&passed->registered);

    start_routine = passed->start_routine;
    arg = passed->arg;
    
    /* Lock our running mutex to allow for future joins */
    pthread_mutex_lock(passed->thread->running_mutex);
    
    /* Call the user routine */
    ret = start_routine(arg);
    
    /* The pointer 'ret' must bo returned to any waiting
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
    
    passed = (struct __thread_pass *)malloc(sizeof(struct __thread_pass));
    if(passed == NULL) {
        _RWD_errno = ENOMEM;
        return -1;
    }
    
    passed->start_routine = start_routine;
    passed->arg = arg;
    passed->thread = NULL;
    
    if(stack == NULL && stack_size > 0) {
        stack = (char *)malloc(stack_size*sizeof(char *));
        if(stack == NULL) {
            free(passed);
            _RWD_errno = ENOMEM;
            return( -1 );
        }
    }
    
    if(sem_init(&passed->registered, 0, 1) != 0) {
        return -1;
    }
    
    sem_wait(&passed->registered);
    
    ret = _beginthread( __thread_start, NULL, 0, (void *)passed );
    
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

    /* Destroy the passing structure */
    free(passed);

    return( ret );
}
