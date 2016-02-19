
#include "variety.h"
#include <pthread.h>
#include <process.h>
#include <malloc.h>
#include "rterrno.h"

/* By default, allow OpenWatcom's thread library
 * to handle this...
 */
#define STACK_SIZE  0

struct __thread_pass {
    void      *(*start_routine)(void*); 
    void      *arg;
    pthread_t *thread;
};

void __thread_start(void *data)
{
struct __thread_pass *passed;
void *ret;

    passed = (struct __thread_pass *)data;
    ret = passed->start_routine(passed->arg);

    passed->thread->

    /* The pointer 'ret' must be returned to any waiting
     * "join" operations
     */
    
    _endthread();
}

_WCRTLINK int pthread_create( pthread_t *thread, const pthread_attr_t *attr,
                              void *(*start_routine)(void*), void *arg )
{
    int ret;
    char *stack = NULL; //(char *)malloc( STACK_SIZE );
    struct __thread_pass *passed;
    
    if(thread == NULL) {
        _RWD_errno = EINVAL;
        return -1;
    }
    
    thread->return_value = NULL;
    
    passed = (struct __thread_pass *)malloc(sizeof(struct __thread_pass *));
    if(passed == NULL) {
        _RWD_errno = ENOMEM;
        return -1;
    }
    
    passed->start_routine = start_routine;
    passed->arg = arg;
    
    ret = _beginthread( __thread_start, stack, STACK_SIZE, (void *)passed );
    
    if(ret >= 0) {
        thread->id = ret;
        thread
        ret = 0;
    }
    
    return ret;
}
