
#include "variety.h"
#include <stdio.h>
#include <pthread.h>
#include <process.h>

#include "_ptint.h"

_WCRTLINK int pthread_join(pthread_t thread, void **value_ptr)
{
int res;

    /* Increment the thread's waiters */
    res = pthread_mutex_lock(__get_thread_waiting_mutex(thread));
    if(res != 0)
        return res;
    __increment_thread_waiters(thread);
    pthread_mutex_unlock(__get_thread_waiting_mutex(thread));

    /* Wait for the thread to release its running lock */
    res = pthread_mutex_lock(__get_thread_running_mutex(thread));
    if(res != 0)
        return res;
    
    /* Copy the "internal only" payload pointer */
    if(value_ptr != NULL) 
        *value_ptr = __get_thread_return_value(thread);

    /* Decrement the waiting count */
    res = pthread_mutex_lock(__get_thread_waiting_mutex(thread));
    if(res == 0) {
        __decrement_thread_waiters(thread);
        pthread_mutex_unlock(__get_thread_waiting_mutex(thread));
    }
    
    /* Unlock it so others may use it */
    pthread_mutex_unlock(__get_thread_running_mutex(thread));
    
    return 0;
}    
